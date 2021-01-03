#include "src/ui/opengl/background.h"

#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "src/ui/opengl/shader.h"

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
  program_->SetVertices({
      // nametable 1
      -1.0, 1.0, 0.0,   // top-left
      1.0, 1.0, 0.0,    // top-right
      1.0, -1.0, 0.0,   // bottom-right
      -1.0, -1.0, 0.0,  // bottom-left

      // nametable 2
      -1.0, -1.0, 0.0,  // top-left
      1.0, -1.0, 0.0,   // top-right
      1.0, -3.0, 0.0,   // bottom-right
      -1.0, -3.0, 0.0,  // bottom-left

      // nametable 1
      -1.0, -3.0, 0.0,  // top-left
      1.0, -3.0, 0.0,   // top-right
      1.0, -5.0, 0.0,   // bottom-right
      -1.0, -5.0, 0.0,  // bottom-left
  });
  program_->SetElements({
      // nametable 1
      0, 1, 2,  // triangle in top-right
      2, 3, 0,  // triangle in bottom-left

      // nametable 2
      4, 5, 6,  // triangle in top-right
      6, 7, 4,  // triangle in bottom-left

      // nametable 1
      8, 9, 10,   // triangle in top-right
      10, 11, 8,  // triangle in bottom-left
  });
  program_->SetTextureCoords({
      // nametable 1
      0.0, 0.0, 0.0,  // bottom-left = tile#0
      1.0, 0.0, 0.0,  // bottom-right
      1.0, 1.0, 0.0,  // top-right
      0.0, 1.0, 0.0,  // top-left

      // nametable 2
      0.0, 0.0, 1.0,  // bottom-left = tile#0
      1.0, 0.0, 1.0,  // bottom-right
      1.0, 1.0, 1.0,  // top-right
      0.0, 1.0, 1.0,  // top-left

      // nametable 1
      0.0, 0.0, 0.0,  // bottom-left = tile#0
      1.0, 0.0, 0.0,  // bottom-right
      1.0, 1.0, 0.0,  // top-right
      0.0, 1.0, 0.0,  // top-left
  });
}
