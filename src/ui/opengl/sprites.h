#ifndef UI_OPENGL_SPRITES_H_
#define UI_OPENGL_SPRITES_H_

#include <memory>

#include "src/ui/opengl/program.h"
#include "src/ui/renderer.h"

class Sprites {
 public:
  Sprites();

  void Draw();
  void SetTiles(std::vector<uint8_t>& tiles);
  void SetPalettes(std::vector<uint8_t>& palettes);
  void SetSprites(std::vector<Sprite>& sprites);

 private:
  void PrepareTextures();

  std::unique_ptr<Program> program_;

  // Textures.
  unsigned int tiles_;
  unsigned int attributes_;
  unsigned int palettes_;
};

#endif  // UI_OPENGL_SPRITES_H_
