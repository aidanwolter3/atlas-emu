#ifndef ENGINE_PPU2_H_
#define ENGINE_PPU2_H_

#include <cstdint>
#include <vector>

#include "src/engine/base/bus.h"
#include "src/engine/cpu.h"
#include "src/ui/renderer2.h"

enum class MirroringMode {
  kOneScreenLower = 0,
  kOneScreenUpper = 1,
  kVertical = 2,
  kHorizontal = 3,
};

class Ppu {
 public:
  virtual ~Ppu() {}

  virtual void Tick() = 0;
  virtual void DumpRegisters() {}
  virtual void SetMirroringMode(MirroringMode mode) {}
};

class PpuImpl : public Ppu, public Peripheral {
 public:
  PpuImpl(Cpu& cpu, Renderer2& renderer);
  ~PpuImpl() override;

  // Ppu implementation:
  void Tick() override;
  void DumpRegisters() override;
  void SetMirroringMode(MirroringMode mode) override;

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;

 private:
  // Draws the pixel at the current |cycle_| and |scanline_| to |frame_|.
  void RenderPixel();

  struct BackgroundPixel {
    uint8_t color_num;
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };
  BackgroundPixel GetBackgroundPixel();

  struct SpritePixel {
    bool valid;
    bool in_background;
    bool is_sprite_zero;
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };
  SpritePixel GetSpritePixel();

  // Tracks the current state of the PPU's ticks. Whether in vlank or drawing
  // pixels.
  int cycle_ = 0;
  int scanline_ = 0;
  bool vblank_ = false;

  // Store the last value written to a register, so that it can be returned in
  // the lowest bits of a read from the PPU status register.
  uint8_t last_write_value_ = 0;

  // Track whether sprite 0 has been hit for this frame.
  bool sprite_0_hit_ = false;

  // The current register values and state.
  bool paired_write_latch_ = true;
  uint8_t ctrl_ = 0;
  uint8_t mask_ = 0;
  uint8_t oam_address_ = 0;
  uint16_t data_address_;
  uint8_t base_nametable_;
  uint8_t scroll_x_ = 0;
  uint8_t scroll_y_ = 0;

  // Data.
  std::vector<uint8_t> oam_;
  std::vector<std::vector<uint8_t>> pattern_;
  std::vector<std::vector<uint8_t>> nametable_;
  std::vector<std::vector<uint8_t>> attribute_;
  std::vector<uint8_t> frame_palette_;

  // Frame sent to the renderer.
  std::vector<uint8_t> frame_;

  Cpu& cpu_;
  Renderer2& renderer_;
  MirroringMode mirroring_mode_;
};

#endif  // ENGINE_PPU2_H_
