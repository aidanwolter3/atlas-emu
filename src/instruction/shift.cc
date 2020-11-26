#include "src/instruction/shift.h"

#include <cstdint>
#include <iostream>

void ASL::ExecuteInternal(uint8_t opcode) {
  uint8_t data_before, data_after;

  if (opcode == 0x0A) {
    data_before = reg_.acc;
    data_after = data_before << 1;
    reg_.acc = data_after;
  } else {
    uint16_t address;
    switch (opcode) {
      case 0x06:
        address = ZeroPage();
        break;
      case 0x16:
        address = IndexedZeroPage(reg_.x);
        break;
      case 0x0E:
        address = Absolute();
        break;
      case 0x1E:
        address = IndexedAbsolute(reg_.x);
        break;
      default:
        std::cout << "Unsupported ASL variant: " << opcode << std::endl;
        return;
    }
    bus_.Read(address, &data_before);
    data_after = data_before << 1;
    bus_.Write(address, data_after);
  }

  reg_.status.set(Status::kCarry, data_before & 0x80);
  reg_.status.set(Status::kZero, data_after == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);
}

void LSR::ExecuteInternal(uint8_t opcode) {
  uint8_t data_before, data_after;

  if (opcode == 0x4A) {
    data_before = reg_.acc;
    data_after = data_before >> 1;
    reg_.acc = data_after;
  } else {
    uint16_t address;
    switch (opcode) {
      case 0x46:
        address = ZeroPage();
        break;
      case 0x56:
        address = IndexedZeroPage(reg_.x);
        break;
      case 0x4E:
        address = Absolute();
        break;
      case 0x5E:
        address = IndexedAbsolute(reg_.x);
        break;
      default:
        std::cout << "Unsupported LSR variant: " << opcode << std::endl;
        return;
    }
    bus_.Read(address, &data_before);
    data_after = data_before >> 1;
    bus_.Write(address, data_after);
  }

  reg_.status.set(Status::kCarry, data_before & 0x01);
  reg_.status.set(Status::kZero, data_after == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);
}

void ROL::ExecuteInternal(uint8_t opcode) {
  uint8_t data_before, data_after;
  uint8_t carry = reg_.status.test(Status::kCarry) ? 0x01 : 0x00;

  if (opcode == 0x2A) {
    data_before = reg_.acc;
    data_after = data_before << 1 | carry;
    reg_.acc = data_after;
  } else {
    uint16_t address;
    switch (opcode) {
      case 0x26:
        address = ZeroPage();
        break;
      case 0x36:
        address = IndexedZeroPage(reg_.x);
        break;
      case 0x2E:
        address = Absolute();
        break;
      case 0x3E:
        address = IndexedAbsolute(reg_.x);
        break;
      default:
        std::cout << "Unsupported ROL variant: " << opcode << std::endl;
        return;
    }
    bus_.Read(address, &data_before);
    data_after = data_before << 1 | carry;
    bus_.Write(address, data_after);
  }

  reg_.status.set(Status::kCarry, data_before & 0x80);
  reg_.status.set(Status::kZero, data_after == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);
}

void ROR::ExecuteInternal(uint8_t opcode) {
  uint8_t data_before, data_after;
  uint8_t carry = reg_.status.test(Status::kCarry) ? 0x80 : 0x00;

  if (opcode == 0x6A) {
    data_before = reg_.acc;
    data_after = data_before >> 1 | carry;
    reg_.acc = data_after;
  } else {
    uint16_t address;
    switch (opcode) {
      case 0x66:
        address = ZeroPage();
        break;
      case 0x76:
        address = IndexedZeroPage(reg_.x);
        break;
      case 0x6E:
        address = Absolute();
        break;
      case 0x7E:
        address = IndexedAbsolute(reg_.x);
        break;
      default:
        std::cout << "Unsupported ROL variant: " << opcode << std::endl;
        return;
    }
    bus_.Read(address, &data_before);
    data_after = data_before >> 1 | carry;
    bus_.Write(address, data_after);
  }

  reg_.status.set(Status::kCarry, data_before & 0x01);
  reg_.status.set(Status::kZero, data_after == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);
}
