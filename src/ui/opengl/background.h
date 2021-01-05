#ifndef UI_OPENGL_BACKGROUND_H_
#define UI_OPENGL_BACKGROUND_H_

#include <cstdint>
#include <vector>

#include "src/ui/opengl/program.h"
#include "src/ui/renderer.h"

class Background {
 public:
  Background();

  void Draw();
  void SetMirroringMode(MirroringMode mode);
  void SetTiles(int num, std::vector<uint8_t>& tiles);
  void SetAttributes(int num, std::vector<uint8_t>& attributes);
  void SetPalettes(std::vector<uint8_t>& palettes);
  void SetScroll(int x, int y);
  void SetVerticalSplit(int scanline, int x, int y);

 private:
  void PrepareTextures();
  void RefreshElements();

  std::unique_ptr<Program> program_;

  // Textures.
  unsigned int tiles_;
  unsigned int attributes_;
  unsigned int palettes_;

  MirroringMode mirroring_mode_;
  int scroll_x_ = 0;
  int scroll_y_ = 0;
  int vertical_split_scanline_ = 0;
  int vertical_split_scroll_x_ = 0;
  int vertical_split_scroll_y_ = 0;
};

#endif  // UI_OPENGL_BACKGROUND_H_
