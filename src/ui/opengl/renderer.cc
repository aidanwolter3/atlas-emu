#include "src/ui/opengl/renderer.h"

#include <iostream>
#include <utility>
#include <vector>

#include <glad/glad.h>
#include "src/ui/opengl/gl_init.h"

OpenGLRenderer::OpenGLRenderer() {
  InitGLIfNeeded();
  background_ = std::make_unique<Background>();
  PrepareTextures();
}

OpenGLRenderer::~OpenGLRenderer() = default;

void OpenGLRenderer::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  background_->Draw();
}

void OpenGLRenderer::SetScroll(int x, int y) { background_->SetScroll(x, y); }

void OpenGLRenderer::SetNametable(int num, std::vector<uint8_t>& nametable) {
  background_->SetTiles(num, nametable);
}

void OpenGLRenderer::SetAttributeTable(int num,
                                       std::vector<uint8_t>& attribute_table) {
  background_->SetAttributes(num, attribute_table);
}

void OpenGLRenderer::SetFramePalette(std::vector<uint8_t>& frame_palette) {
  if (frame_palette.size() != 32) {
    std::cout << "Invalid number of palettes" << std::endl;
    return;
  }
  std::vector<uint8_t> background_palette(frame_palette.begin(),
                                          frame_palette.begin() + 16);
  background_->SetPalettes(background_palette);
}

void OpenGLRenderer::SetPalette(std::vector<uint8_t>& palette) {
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palette_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, palette.size() / 3, 0, GL_RGB,
               GL_UNSIGNED_BYTE, palette.data());
}

void OpenGLRenderer::PrepareTextures() {
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
