#include "src/instruction/shift.h"

#include <cstdint>
#include <iostream>

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
      bus_.Read(address, &data_before);
      data_after = data_before << 1;
      bus_.Write(address, data_after);
      break;
    case 0x16:
      address = IndexedZeroPage(reg_.x);
      bus_.Read(address, &data_before);
      data_after = data_before << 1;
      bus_.Write(address, data_after);
      break;
    case 0x0E:
      address = Absolute();
      bus_.Read(address, &data_before);
      data_after = data_before << 1;
      bus_.Write(address, data_after);
      break;
    case 0x1E:
      address = IndexedAbsolute(reg_.x);
      bus_.Read(address, &data_before);
      data_after = data_before << 1;
      bus_.Write(address, data_after);
      break;
    default:
      std::cout << "Unsupported ASL variant: " << opcode << std::endl;
      return;
  }
  reg_.status.set(Status::kCarry, data_before & 0x80);
  reg_.status.set(Status::kZero, data_after == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);
}

void LSR::ExecuteInternal(uint8_t opcode) {
  uint8_t data_before, data_after;
  uint16_t address;
  switch (opcode) {
    case 0x4A:
      data_before = reg_.acc;
      data_after = data_before >> 1;
      reg_.acc = data_after;
      break;
    case 0x46:
      address = ZeroPage();
      bus_.Read(address, &data_before);
      data_after = data_before >> 1;
      bus_.Write(address, data_after);
      break;
    case 0x56:
      address = IndexedZeroPage(reg_.x);
      bus_.Read(address, &data_before);
      data_after = data_before >> 1;
      bus_.Write(address, data_after);
      break;
    case 0x4E:
      address = Absolute();
      bus_.Read(address, &data_before);
      data_after = data_before >> 1;
      bus_.Write(address, data_after);
      break;
    case 0x5E:
      address = IndexedAbsolute(reg_.x);
      bus_.Read(address, &data_before);
      data_after = data_before >> 1;
      bus_.Write(address, data_after);
      break;
    default:
      std::cout << "Unsupported LSR variant: " << opcode << std::endl;
      return;
  }
  reg_.status.set(Status::kCarry, data_before & 0x01);
  reg_.status.set(Status::kZero, data_after == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);
}
