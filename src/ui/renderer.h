#ifndef UI_RENDERER_H_
#define UI_RENDERER_H_

#include <cstdint>
#include <vector>

struct Sprite {
  uint8_t x, y;
  uint8_t tile_num;
  uint8_t palette;
};

class Renderer {
 public:
  virtual ~Renderer() {}
  virtual void Render() = 0;
  virtual void SetScroll(int x, int y) = 0;
  virtual void SetNametable(int num, std::vector<uint8_t>& nametable) = 0;
  virtual void SetAttributeTable(int num,
                                 std::vector<uint8_t>& attribute_table) = 0;
  virtual void SetFramePalette(std::vector<uint8_t>& frame_palette) = 0;
  virtual void SetPalette(std::vector<uint8_t>& palette) = 0;
  virtual void SetSpriteTiles(std::vector<uint8_t>& tiles) = 0;
  virtual void SetSprites(std::vector<Sprite>& sprites) = 0;
};

#endif  // UI_RENDERER_H_
