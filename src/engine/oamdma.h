#ifndef ENGINE_OAMDMA_H_
#define ENGINE_OAMDMA_H_

#include "src/public/bus.h"

class OAMDMA : public Peripheral {
 public:
  OAMDMA(Bus& bus);
  ~OAMDMA() override;

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;

 private:
  Bus& bus_;
};

#endif  // ENGINE_OAMDMA_H_
