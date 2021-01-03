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

std::vector<float> CreateBgVertices(int x, int y, float depth) {
  float x_offset = 2.0f * x;
  float y_offset = -2.0f * y;
  std::vector<float> vertices = {
      x_offset - 1.0f, y_offset + 1.0f, depth,  // top-left
      x_offset + 1.0f, y_offset + 1.0f, depth,  // top-right
      x_offset + 1.0f, y_offset - 1.0f, depth,  // bottom-right
      x_offset - 1.0f, y_offset - 1.0f, depth,  // bottom-left
  };
  return vertices;
}

std::vector<float> CreateBgTextureCoords(int table_num) {
  std::vector<float> texture_coords = {
      0.0, 0.0, (float)table_num,  // bottom-left
      1.0, 0.0, (float)table_num,  // bottom-right
      1.0, 1.0, (float)table_num,  // top-right
      0.0, 1.0, (float)table_num,  // top-left
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
  LoadElements();
  SetScroll(0, 0);
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

  // Set the scroll.
  glm::mat4 transform = glm::mat4(1.0f);
  float x = 2.0f * scroll_x_ / float(0x100);
  float y = 2.0f * scroll_y_ / float(0xF0);
  transform = glm::translate(transform, glm::vec3(-x, y, 0.0f));
  auto transform_loc = program_->GetUniformLocation("transform");
  glUniformMatrix4fv(transform_loc, 1, false, &transform[0][0]);

  program_->Draw();
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
  scroll_x_ = x % 0x200;
  scroll_y_ = y % 0x200;
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

void Background::LoadElements() {
  std::vector<unsigned int> elements;
  std::vector<float> vertices;
  std::vector<float> texture_coords;

  // Table 1: (0, 0)
  AppendVectorTo(elements, CreateElements(0));
  AppendVectorTo(vertices, CreateBgVertices(0, 0, 1.0));  // Zero color
  AppendVectorTo(texture_coords, CreateBgTextureCoords(0));
  AppendVectorTo(elements, CreateElements(4));
  AppendVectorTo(vertices, CreateBgVertices(0, 0, 0.0));  // Non-zero color
  AppendVectorTo(texture_coords, CreateBgTextureCoords(0));

  // Table 2: (0, 1)
  AppendVectorTo(elements, CreateElements(8));
  AppendVectorTo(vertices, CreateBgVertices(0, 1, 1.0));  // Zero color
  AppendVectorTo(texture_coords, CreateBgTextureCoords(1));
  AppendVectorTo(elements, CreateElements(12));
  AppendVectorTo(vertices, CreateBgVertices(0, 1, 0.0));  // Non-zero color
  AppendVectorTo(texture_coords, CreateBgTextureCoords(1));

  // Table 1: (0, 2)
  AppendVectorTo(elements, CreateElements(16));
  AppendVectorTo(vertices, CreateBgVertices(0, 2, 1.0));  // Zero color
  AppendVectorTo(texture_coords, CreateBgTextureCoords(0));
  AppendVectorTo(elements, CreateElements(20));
  AppendVectorTo(vertices, CreateBgVertices(0, 2, 0.0));  // Non-zero color
  AppendVectorTo(texture_coords, CreateBgTextureCoords(0));

  program_->SetElements(elements);
  program_->SetVertices(vertices);
  program_->SetTextureCoords(texture_coords);
}
