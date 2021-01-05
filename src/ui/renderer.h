#ifndef UI_RENDERER_H_
#define UI_RENDERER_H_

#include <cstdint>
#include <vector>

enum class MirroringMode {
  kOneScreenLower = 0,
  kOneScreenUpper = 1,
  kVertical = 2,
  kHorizontal = 3,
};

struct Sprite {
  uint8_t x, y;
  uint8_t attribute;
  std::vector<uint8_t> tile;
};

class Renderer {
 public:
  virtual ~Renderer() {}
  virtual void Render() = 0;
  virtual void SetMirroringMode(MirroringMode mode) = 0;
  virtual void SetMask(uint8_t mask) = 0;
  virtual void SetScroll(int x, int y) = 0;
  virtual void SetVerticalSplit(int scanline, int x, int y) = 0;
  virtual void SetNametable(int num, std::vector<uint8_t>& nametable) = 0;
  virtual void SetAttributeTable(int num,
                                 std::vector<uint8_t>& attribute_table) = 0;
  virtual void SetFramePalette(std::vector<uint8_t>& frame_palette) = 0;
  virtual void SetPalette(std::vector<uint8_t>& palette) = 0;
  virtual void SetSprites(std::vector<Sprite>& sprites) = 0;
};

class FakeRenderer : public Renderer {
 public:
  // Renderer implementation:
  void Render() override {}
  void SetMirroringMode(MirroringMode mode) override {}
  void SetMask(uint8_t mask) override {}
  void SetScroll(int x, int y) override {}
  void SetVerticalSplit(int scanline, int x, int y) override {}
  void SetNametable(int num, std::vector<uint8_t>& nametable) override {}
  void SetAttributeTable(int num,
                         std::vector<uint8_t>& attribute_table) override {}
  void SetFramePalette(std::vector<uint8_t>& frame_palette) override {}
  void SetPalette(std::vector<uint8_t>& palette) override {}
  void SetSprites(std::vector<Sprite>& sprites) override {}
};

#endif  // UI_RENDERER_H_
