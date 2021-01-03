#include "src/ui/opengl/renderer.h"

#include <iostream>
#include <utility>
#include <vector>

#include <glad/glad.h>
#include "src/ui/opengl/gl_init.h"

OpenGLRenderer::OpenGLRenderer() {
  InitGLIfNeeded();
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0f);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  background_ = std::make_unique<Background>();
  sprites_ = std::make_unique<Sprites>();
  PrepareTextures();
}

OpenGLRenderer::~OpenGLRenderer() = default;

void OpenGLRenderer::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (show_background) {
    background_->Draw();
  }
  if (show_sprites) {
    sprites_->Draw();
  }

  // Bind the shared palette to the correct texture unit.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_1D, palette_);
}

void OpenGLRenderer::SetMask(uint8_t mask) {
  show_background = (mask & 0x08);
  show_sprites = (mask & 0x10);
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
  std::vector<uint8_t> sprite_palette(frame_palette.begin() + 16,
                                      frame_palette.begin() + 32);

  background_->SetPalettes(background_palette);
  sprites_->SetPalettes(sprite_palette);
}

void OpenGLRenderer::SetPalette(std::vector<uint8_t>& palette) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_1D, palette_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, palette.size() / 3, 0, GL_RGB,
               GL_UNSIGNED_BYTE, palette.data());
}

void OpenGLRenderer::SetSprites(std::vector<Sprite>& sprites) {
  sprites_->SetSprites(sprites);
}

void OpenGLRenderer::PrepareTextures() {
  // Declare the palette.
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &palette_);
  glBindTexture(GL_TEXTURE_1D, palette_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glActiveTexture(0);
}
