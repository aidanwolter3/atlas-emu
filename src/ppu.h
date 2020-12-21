#ifndef PPU_H_
#define PPU_H_

#include <unordered_set>
#include <vector>

#include "src/cpu.h"
#include "src/public/bus.h"
#include "src/public/clock.h"
#include "src/ui/window.h"

class Ppu : public Peripheral, public Clock::TimerObserver {
 public:
  Ppu(Clock& clock, Cpu& cpu, Window& window);
  ~Ppu() override;

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;

  // Clock::TimerObserver implementation:
  void OnTimerCalled() override;

 private:
  void LoadTile(int num, uint8_t tile_num, uint8_t palette);

  Cpu& cpu_;
  Window& window_;

  // Store the last value written to a register, so that it can be returned in
  // the lowest bits of a read from the PPU status register.
  uint8_t last_write_value_ = 0;

  uint8_t ctrl_ = 0;
  uint8_t mask_ = 0;
  uint8_t oam_address_ = 0;

  bool scroll_write_to_x_ = true;
  uint8_t scroll_x_ = 0;
  uint8_t scroll_y_ = 0;

  bool address_write_to_upper_ = true;
  uint16_t address_;

  std::vector<uint8_t> vram_;
};

#endif  // PPU_H_
