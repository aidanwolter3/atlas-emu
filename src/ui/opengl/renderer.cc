#include "src/ui/opengl/renderer.h"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "src/ui/opengl/gl_init.h"
#include "src/ui/opengl/shader.h"

OpenGLRenderer::OpenGLRenderer() {
  InitGLIfNeeded();

  // Create the Shaders and Program.
  std::vector<std::unique_ptr<Shader>> shaders;
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_VERTEX_SHADER, "src/ui/opengl/vertex.glsl")));
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_FRAGMENT_SHADER, "src/ui/opengl/fragment.glsl")));
  program_ = std::make_unique<Program>(std::move(shaders));

  PrepareUniforms();
  LoadElements();
  SetScroll(0, 0);
}

OpenGLRenderer::~OpenGLRenderer() = default;

void OpenGLRenderer::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  program_->Draw();
}

void OpenGLRenderer::SetScroll(int x, int y) {
  x = x % 0x200;
  y = y % 0x200;
  glm::mat4 transform = glm::mat4(1.0f);
  float scroll_x = 2.0f * x / float(0x100);
  float scroll_y = 2.0f * y / float(0xF0);
  transform = glm::translate(transform, glm::vec3(-scroll_x, scroll_y, 0.0f));
  auto transform_loc = program_->GetUniformLocation("transform");
  glUniformMatrix4fv(transform_loc, 1, false, &transform[0][0]);
}

void OpenGLRenderer::SetNametable(int num, std::vector<uint8_t>& nametable) {
  if (nametable.size() != (32 * 8 * 30 * 8)) {
    std::cout << "Nametable is the wrong size" << std::endl;
    return;
  }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, nametable_);
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0,
                  /*index=*/num,
                  /*width=*/32 * 8,
                  /*height=*/30 * 8, 1, GL_RED_INTEGER, GL_UNSIGNED_BYTE,
                  nametable.data());
}

void OpenGLRenderer::SetAttributeTable(int num,
                                       std::vector<uint8_t>& attribute_table) {
  if (attribute_table.size() != 0x40) {
    std::cout << "Attribute table is the wrong size" << std::endl;
    return;
  }
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_1D_ARRAY, attribute_table_);
  glTexSubImage2D(GL_TEXTURE_1D_ARRAY, 0, 0,
                  /*index=*/num,
                  /*width=*/0x40, 1, GL_RED_INTEGER, GL_UNSIGNED_BYTE,
                  attribute_table.data());
}

void OpenGLRenderer::SetFramePalette(std::vector<uint8_t>& frame_palette) {
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_1D, frame_palette_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, frame_palette.size(), 0,
               GL_RED_INTEGER, GL_UNSIGNED_BYTE, frame_palette.data());
}

void OpenGLRenderer::SetPalette(std::vector<uint8_t>& palette) {
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palette_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, palette.size() / 3, 0, GL_RGB,
               GL_UNSIGNED_BYTE, palette.data());
}

void OpenGLRenderer::PrepareUniforms() {
  // Set the uniform locations.
  program_->Use();
  glUniform1i(program_->GetUniformLocation("nametable"), 0);
  glUniform1i(program_->GetUniformLocation("attribute_table"), 1);
  glUniform1i(program_->GetUniformLocation("frame_palette"), 2);
  glUniform1i(program_->GetUniformLocation("palette"), 3);

  // Declare the tile textures.
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &nametable_);
  glBindTexture(GL_TEXTURE_2D_ARRAY, nametable_);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8UI,
               /*width=*/32 * 8,
               /*height=*/30 * 8,
               /*count=*/2, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);

  // Declare the attribute table.
  glActiveTexture(GL_TEXTURE1);
  glGenTextures(1, &attribute_table_);
  glBindTexture(GL_TEXTURE_1D_ARRAY, attribute_table_);
  glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_1D_ARRAY, 0, GL_R8UI,
               /*width=*/0x40,
               /*count=*/2, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);

  // Declare the frame palette.
  glActiveTexture(GL_TEXTURE2);
  glGenTextures(1, &frame_palette_);
  glBindTexture(GL_TEXTURE_1D, frame_palette_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Declare the palette.
  glActiveTexture(GL_TEXTURE3);
  glGenTextures(1, &palette_);
  glBindTexture(GL_TEXTURE_1D, palette_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glActiveTexture(0);
}

void OpenGLRenderer::LoadElements() {
  program_->SetVertices({
      // nametable 1
      -1.0, 1.0,   // top-left
      1.0, 1.0,    // top-right
      1.0, -1.0,   // bottom-right
      -1.0, -1.0,  // bottom-left

      // nametable 2
      -1.0, -1.0,  // top-left
      1.0, -1.0,   // top-right
      1.0, -3.0,   // bottom-right
      -1.0, -3.0,  // bottom-left

      // nametable 1
      -1.0, -3.0,  // top-left
      1.0, -3.0,   // top-right
      1.0, -5.0,   // bottom-right
      -1.0, -5.0,  // bottom-left
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
