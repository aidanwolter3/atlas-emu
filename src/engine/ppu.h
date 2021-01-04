#ifndef ENGINE_PPU_H_
#define ENGINE_PPU_H_

#include <optional>
#include <unordered_set>
#include <vector>

#include "src/engine/cpu.h"
#include "src/engine/public/bus.h"
#include "src/ui/renderer.h"

class Ppu {
 public:
  virtual ~Ppu() {}

  virtual void Scanline() = 0;
  virtual void Render() = 0;
  virtual void DumpRegisters() {}
  virtual void SetMirroringMode(MirroringMode mode) {}
};

class PpuImpl : public Ppu, public Peripheral {
 public:
  PpuImpl(Cpu& cpu, Renderer& renderer);
  ~PpuImpl() override;

  // Ppu implementation:
  void Scanline() override;
  void Render() override;
  void DumpRegisters() override;
  void SetMirroringMode(MirroringMode mode) override;

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;

 private:
  void LoadBackground();
  void LoadSprites();
  void DetectSprite0Hit();

  Cpu& cpu_;
  Renderer& renderer_;
  MirroringMode mirroring_mode_;

  int scan_line_ = 0;
  bool vblank_ = false;
  bool sprite_0_hit_ = false;

  // Store the last value written to a register, so that it can be returned in
  // the lowest bits of a read from the PPU status register.
  uint8_t last_write_value_ = 0;

  // Registers
  uint8_t ctrl_ = 0;
  uint8_t mask_ = 0;
  uint8_t oam_address_ = 0;

  bool scroll_write_to_x_ = true;
  uint8_t scroll_x_ = 0;
  uint8_t scroll_y_ = 0;

  bool address_write_to_upper_ = true;
  uint16_t data_address_;

  // Data
  std::vector<uint8_t> oam_;
  std::vector<std::vector<uint8_t>> pattern_;
  std::vector<std::vector<uint8_t>> nametable_;
  std::vector<std::vector<uint8_t>> attribute_;
  std::vector<uint8_t> frame_palette_;
  bool oam_dirty_ = false;
  bool pattern_dirty_ = true;
  bool nametable_dirty_ = true;
  bool attribute_dirty_ = true;
  bool frame_palette_dirty_ = true;

  // Computed data used for sprite 0 hit dectection.
  std::optional<Sprite> sprite_0_;
  std::vector<std::vector<uint8_t>> background_;
};

#endif  // ENGINE_PPU_H_
