#include "src/ui/opengl/sprites.h"

#include <iostream>
#include <utility>

namespace {

// Inserts all the elements of |src| to the end of |dest|.
template <class T>
void AppendVectorTo(std::vector<T>& dest, std::vector<T> src) {
  dest.insert(dest.end(), src.begin(), src.end());
}

std::vector<unsigned int> CreateSpriteElements(unsigned int first_vertex) {
  std::vector<unsigned int> elements = {
      first_vertex,     first_vertex + 1, first_vertex + 2,
      first_vertex + 2, first_vertex + 3, first_vertex,
  };
  return elements;
}

std::vector<float> CreateSpriteVertices(Sprite& s) {
  const int height = (s.tile.size() == 128 ? 16 : 8);
  float unit_x = 2.0 * 8 / 0x100;
  float unit_y = 2.0 * height / 0xF0;
  float top_left_x = ((float)s.x * 2.0 / 0x100) - 1.0;
  float top_left_y = ((float)(0xF0 - s.y) * 2.0 / 0xF0) - 1.0;

  std::vector<float> vertices = {
      top_left_x,          top_left_y,          1.0,
      top_left_x + unit_x, top_left_y,          1.0,
      top_left_x + unit_x, top_left_y - unit_y, 1.0,
      top_left_x,          top_left_y - unit_y, 1.0,
  };
  return vertices;
}

std::vector<float> CreateSpriteTextureCoords(Sprite& s, int sprite_num) {
  const int height = (s.tile.size() == 128 ? 16 : 8);
  const bool flipx = s.attribute & 0x40;
  const bool flipy = s.attribute & 0x80;
  float left_x = 0.0;
  float bottom_y = 0.0;
  float right_x = 1.0;
  float top_y = 1.0;
  if (height == 8) {
    top_y = 0.5;
  }
  if (flipy) {
    std::swap(bottom_y, top_y);
  }
  if (flipx) {
    std::swap(left_x, right_x);
  }

  float num = sprite_num;
  std::vector<float> texture_coords = {
      left_x,  bottom_y, num,  // bottom-left
      right_x, bottom_y, num,  // bottom-right
      right_x, top_y,    num,  // top-right
      left_x,  top_y,    num,  // top-left
  };
  return texture_coords;
}

}  // namespace

Sprites::Sprites() {
  // Create the Shaders and Program.
  std::vector<std::unique_ptr<Shader>> shaders;
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_VERTEX_SHADER, "src/ui/opengl/sprites.vert")));
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_FRAGMENT_SHADER, "src/ui/opengl/sprites.frag")));
  program_ = std::make_unique<Program>(std::move(shaders));

  PrepareTextures();
}

void Sprites::Draw() {
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
  glBindTexture(GL_TEXTURE_1D, attributes_);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palettes_);

  program_->Draw();
}

void Sprites::SetPalettes(std::vector<uint8_t>& palettes) {
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palettes_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, palettes.size(), 0, GL_RED_INTEGER,
               GL_UNSIGNED_BYTE, palettes.data());
}

void Sprites::SetSprites(std::vector<Sprite>& sprites) {
  if (sprites.size() != 64) {
    std::cout << "Invalid number of sprites" << std::endl;
    return;
  }

  std::vector<uint8_t> attributes;
  std::vector<float> vertices;
  std::vector<unsigned int> elements;
  std::vector<float> texture_coords;

  // Bind the tiles.
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tiles_);

  // For each sprite...
  for (int i = 0; (size_t)i < sprites.size(); ++i) {
    auto& s = sprites[i];
    if (s.tile.size() != 64 && s.tile.size() != 128) {
      continue;
    }

    // Set the tile.
    // Note: We cannot do this outside the loop, because for 8x8 tiles we do not
    // fill the entire texture.
    const int height = (s.tile.size() == 128 ? 16 : 8);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                    /*xoffset=*/0,
                    /*yoffset=*/0,
                    /*zoffset=*/i,
                    /*width=*/8,
                    /*height=*/height,
                    /*depth=*/1, GL_RED_INTEGER, GL_UNSIGNED_BYTE,
                    s.tile.data());

    // Set the attributes.
    attributes.push_back(s.attribute);

    // Set the vertices/elements/texture coords.
    AppendVectorTo(elements, CreateSpriteElements(/*first_vertex=*/i * 4));
    AppendVectorTo(vertices, CreateSpriteVertices(s));
    AppendVectorTo(texture_coords, CreateSpriteTextureCoords(s, i));
  }

  // Bind the attributes.
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_1D, attributes_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, attributes.size(), 0, GL_RED_INTEGER,
               GL_UNSIGNED_BYTE, attributes.data());

  // Send the vectors to the program.
  program_->SetVertices(vertices);
  program_->SetElements(elements);
  program_->SetTextureCoords(texture_coords);
}

void Sprites::PrepareTextures() {
  // Declare the tiles.
  // Note: Use a size of 8x16, so that it works for both 8x8 and 8x16 mode. In
  // 8x8 mode, the bottom half will not be used.
  glGenTextures(1, &tiles_);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tiles_);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8UI,
               /*width=*/8,
               /*height=*/16,
               /*count=*/64, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);

  // Declare the attributes.
  glGenTextures(1, &attributes_);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_1D, attributes_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Declare the palettes.
  glGenTextures(1, &palettes_);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palettes_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glActiveTexture(0);
}
