#include "src/instruction/shift.h"

#include <cstdint>
#include <iostream>

namespace {

void SetStatusFromData(Registers& reg, uint8_t data_before, uint8_t data_after) {
  reg.status.set(Status::kCarry, data_before & 0x80);
  reg.status.set(Status::kZero, data_after == 0);
  reg.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);
}

}

void ASL::ExecuteInternal(uint8_t opcode) {
  uint8_t data_before, data_after;
  uint16_t address;
  switch (opcode) {
    case 0x0A:
      data_before = reg_.acc;
      data_after = data_before << 1;
      reg_.acc = data_after;
      break;
    case 0x06:
      address = ZeroPage();
      mem_.Read(address, &data_before);
      data_after = data_before << 1;
      mem_.Write(address, data_after);
      break;
    case 0x16:
      address = IndexedZeroPage(reg_.x);
      mem_.Read(address, &data_before);
      data_after = data_before << 1;
      mem_.Write(address, data_after);
      break;
    case 0x0E:
      address = Absolute();
      mem_.Read(address, &data_before);
      data_after = data_before << 1;
      mem_.Write(address, data_after);
      break;
    case 0x1E:
      address = IndexedAbsolute(reg_.x);
      mem_.Read(address, &data_before);
      data_after = data_before << 1;
      mem_.Write(address, data_after);
      break;
    default:
      std::cout << "Unsupported ASL variant: " << opcode << std::endl;
      return;
  }
  SetStatusFromData(reg_, data_before, data_after);
}
