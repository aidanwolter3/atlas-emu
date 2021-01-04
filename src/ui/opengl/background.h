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

 private:
  void PrepareTextures();
  void LoadElements(int x, int y);

  std::unique_ptr<Program> program_;

  // Textures.
  unsigned int tiles_;
  unsigned int attributes_;
  unsigned int palettes_;

  MirroringMode mirroring_mode_;
};

#endif  // UI_OPENGL_BACKGROUND_H_
