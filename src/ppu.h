#ifndef PPU_H_
#define PPU_H_

#include "src/public/bus.h"

class Ppu : public Peripheral {
 public:
  Ppu();
  ~Ppu();

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;
};

#endif  // PPU_H_
