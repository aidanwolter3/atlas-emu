#include "src/ui/opengl/background.h"

#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "src/ui/opengl/shader.h"

namespace {

// Inserts all the elements of |src| to the end of |dest|.
template <class T>
void AppendVectorTo(std::vector<T>& dest, std::vector<T> src) {
  dest.insert(dest.end(), src.begin(), src.end());
}

std::vector<unsigned int> CreateElements(unsigned int first_vertex) {
  std::vector<unsigned int> elements = {
      first_vertex,     first_vertex + 1, first_vertex + 2,
      first_vertex + 2, first_vertex + 3, first_vertex,
  };
  return elements;
}

// Create the vertices for the background.
// |depth| = 1.0 means zero color.
// |depth| = 0.0 means non-zero color.
std::vector<float> CreateBgVertices(int offset_y, int height, float depth) {
  float y_top = 1.0 - (offset_y * 2.0 / 0xF0);
  float y_bottom = y_top - (height * 2.0 / 0xF0);
  std::vector<float> vertices = {
      -1.0f, y_top,    depth,  // top-left
      +1.0f, y_top,    depth,  // top-right
      +1.0f, y_bottom, depth,  // bottom-right
      -1.0f, y_bottom, depth,  // bottom-left
  };
  return vertices;
}

// Create the texture coordinates for a pane with height |h| pixels starting at
// the texture origin |x| * |y| in pixels from the top-left.
std::vector<float> CreateBgTextureCoords(int x, int y, int h) {
  if (h > 0xF0) {
    std::cout << "Background cannot be larger than the screen size"
              << std::endl;
  }

  float origin_x = (float)(x % 0x300) / 0x100;
  float origin_y = (float)(y % 0x2D0) / 0xF0;
  float width = 1.0;
  float height = (float)h / 0xF0;

  // Note: The 0.0 at the end of each coordinate is unused, but since we share
  // the Program code with the sprites, we use a width of 3 floats.
  std::vector<float> texture_coords = {
      origin_x,         origin_y,          0.0,  // bottom-left
      origin_x + width, origin_y,          0.0,  // bottom-right
      origin_x + width, origin_y + height, 0.0,  // top-right
      origin_x,         origin_y + height, 0.0,  // top-left
  };
  return texture_coords;
}

}  // namespace

Background::Background() {
  // Create the Shaders and Program.
  std::vector<std::unique_ptr<Shader>> shaders;
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_VERTEX_SHADER, "src/ui/opengl/background.vert")));
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_FRAGMENT_SHADER, "src/ui/opengl/background.frag")));
  program_ = std::make_unique<Program>(std::move(shaders));

  PrepareTextures();
  SetMirroringMode(MirroringMode::kHorizontal);
  RefreshElements();
}

void Background::Draw() {
  program_->Use();

  // Assign texture units to the uniform locations.
  glUniform1i(program_->GetUniformLocation("palette"), 0);
  glUniform1i(program_->GetUniformLocation("tiles"), 1);
  glUniform1i(program_->GetUniformLocation("attributes"), 2);
  glUniform1i(program_->GetUniformLocation("palettes"), 3);

  // Bind the textures to the texture units.
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tiles_);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_1D_ARRAY, attributes_);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palettes_);

  // Set the mirroring mode.
  // Currently, we only support horizontal/vertical mirroring.
  bool vertical_mirroring = (mirroring_mode_ == MirroringMode::kVertical);
  auto mirroring_loc = program_->GetUniformLocation("vertical_mirroring");
  glUniform1i(mirroring_loc, vertical_mirroring);

  program_->Draw();
}

void Background::SetMirroringMode(MirroringMode mode) {
  mirroring_mode_ = mode;
}

void Background::SetTiles(int num, std::vector<uint8_t>& tiles) {
  if (tiles.size() != (32 * 8 * 30 * 8)) {
    std::cout << "Invalid number of tiles" << std::endl;
    return;
  }
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tiles_);
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0,
                  /*index=*/num,
                  /*width=*/32 * 8,
                  /*height=*/30 * 8, 1, GL_RED_INTEGER, GL_UNSIGNED_BYTE,
                  tiles.data());
}

void Background::SetAttributes(int num, std::vector<uint8_t>& attributes) {
  if (attributes.size() != 0x40) {
    std::cout << "Invalid number of attributes" << std::endl;
    return;
  }
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_1D_ARRAY, attributes_);
  glTexSubImage2D(GL_TEXTURE_1D_ARRAY, 0, 0,
                  /*index=*/num,
                  /*width=*/0x40, 1, GL_RED_INTEGER, GL_UNSIGNED_BYTE,
                  attributes.data());
}

void Background::SetPalettes(std::vector<uint8_t>& palettes) {
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palettes_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, palettes.size(), 0, GL_RED_INTEGER,
               GL_UNSIGNED_BYTE, palettes.data());
}

void Background::SetScroll(int x, int y) {
  scroll_x_ = x;
  scroll_y_ = y;
  RefreshElements();
}

void Background::SetVerticalSplit(int scanline, int x, int y) {
  if (vertical_split_scanline_ != 0 && scanline == 0) {
    vertical_split_scanline_ = 0;
    RefreshElements();
  } else if (scanline != 0) {
    std::cout << "scanline=" << scanline << std::endl;
    vertical_split_scanline_ = scanline;
    vertical_split_scroll_x_ = x;
    vertical_split_scroll_y_ = y;
    RefreshElements();
  }
}

void Background::PrepareTextures() {
  // Declare the tiles.
  glActiveTexture(GL_TEXTURE1);
  glGenTextures(1, &tiles_);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tiles_);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8UI,
               /*width=*/32 * 8,
               /*height=*/30 * 8,
               /*count=*/2, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);

  // Declare the attributes.
  glActiveTexture(GL_TEXTURE2);
  glGenTextures(1, &attributes_);
  glBindTexture(GL_TEXTURE_1D_ARRAY, attributes_);
  glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_1D_ARRAY, 0, GL_R8UI,
               /*width=*/0x40,
               /*count=*/2, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);

  // Declare the palettes.
  glActiveTexture(GL_TEXTURE3);
  glGenTextures(1, &palettes_);
  glBindTexture(GL_TEXTURE_1D, palettes_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glActiveTexture(0);
}

void Background::RefreshElements() {
  // TODO: This is a lot of redrawing. We should optimize.

  std::vector<unsigned int> elements;
  std::vector<float> vertices;
  std::vector<float> texture_coords;

  int top_split_height =
      vertical_split_scanline_ ? vertical_split_scanline_ : 0xF0;

  // Always add the top split even if it takes up the entire screen.
  AppendVectorTo(elements, CreateElements(0));
  AppendVectorTo(elements, CreateElements(4));
  AppendVectorTo(vertices,
                 CreateBgVertices(0, top_split_height, /*depth=*/1.0));
  AppendVectorTo(vertices,
                 CreateBgVertices(0, top_split_height, /*depth=*/0.0));
  AppendVectorTo(texture_coords,
                 CreateBgTextureCoords(scroll_x_, scroll_y_, top_split_height));
  AppendVectorTo(texture_coords,
                 CreateBgTextureCoords(scroll_x_, scroll_y_, top_split_height));

  // Draw the bottom split if required.
  if (vertical_split_scanline_) {
    AppendVectorTo(elements, CreateElements(8));
    AppendVectorTo(elements, CreateElements(12));
    int bottom_split_height = 0xF0 - top_split_height;
    AppendVectorTo(vertices,
                   CreateBgVertices(top_split_height, bottom_split_height,
                                    /*depth=*/1.0));
    AppendVectorTo(vertices,
                   CreateBgVertices(top_split_height, bottom_split_height,
                                    /*depth=*/0.0));
    AppendVectorTo(
        texture_coords,
        CreateBgTextureCoords(vertical_split_scroll_x_,
                              vertical_split_scroll_y_, bottom_split_height));
    AppendVectorTo(
        texture_coords,
        CreateBgTextureCoords(vertical_split_scroll_x_,
                              vertical_split_scroll_y_, bottom_split_height));
  }

  program_->SetElements(elements);
  program_->SetVertices(vertices);
  program_->SetTextureCoords(texture_coords);
}
