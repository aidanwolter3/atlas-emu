#ifndef UI_OPENGL_BACKGROUND_H_
#define UI_OPENGL_BACKGROUND_H_

#include <cstdint>
#include <vector>

#include "src/ui/opengl/program.h"

class Background {
 public:
  Background();

  void Draw();
  void SetTiles(int num, std::vector<uint8_t>& tiles);
  void SetAttributes(int num, std::vector<uint8_t>& attributes);
  void SetPalettes(std::vector<uint8_t>& palettes);
  void SetScroll(int x, int y);

 private:
  void PrepareTextures();
  void LoadElements();

  std::unique_ptr<Program> program_;

  // Textures.
  unsigned int tiles_;
  unsigned int attributes_;
  unsigned int palettes_;

  int scroll_x_, scroll_y_ = 0;
};

#endif  // UI_OPENGL_BACKGROUND_H_
