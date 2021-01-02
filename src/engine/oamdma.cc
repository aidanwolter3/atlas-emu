#include "src/engine/oamdma.h"

#include <iostream>

OAMDMA::OAMDMA(Bus& bus) : bus_(bus) {}
OAMDMA::~OAMDMA() = default;

Peripheral::Status OAMDMA::Read(uint16_t address, uint8_t* byte) {
  return Peripheral::Status::WRITE_ONLY;
}

Peripheral::Status OAMDMA::Write(uint16_t address, uint8_t byte) {
  if (address != 0) {
    return Peripheral::Status::OUT_OF_BOUNDS;
  }

  uint16_t start_address = byte << 8;
  for (int i = 0; i < 0x100; ++i) {
    uint8_t byte;
    bus_.Read(start_address + i, &byte);
    bus_.Write(0x2004, byte);
  }

  return Peripheral::Status::OK;
}

uint16_t OAMDMA::GetAddressLength() { return 1; }
