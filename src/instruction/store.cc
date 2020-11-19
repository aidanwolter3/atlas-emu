#include "src/instruction/store.h"

#include <iostream>

void STA::ExecuteInternal(uint8_t opcode) {
  switch (opcode) {
    case 0x85:
      bus_.Write(ZeroPage(), reg_.acc);
      break;
    case 0x95:
      bus_.Write(IndexedZeroPage(reg_.x), reg_.acc);
      break;
    case 0x8D:
      bus_.Write(Absolute(), reg_.acc);
      break;
    case 0x9D:
      bus_.Write(IndexedAbsolute(reg_.x), reg_.acc);
      break;
    case 0x99:
      bus_.Write(IndexedAbsolute(reg_.y), reg_.acc);
      break;
    case 0x81:
      bus_.Write(IndexedIndirect(reg_.x), reg_.acc);
      break;
    case 0x91:
      bus_.Write(IndirectIndexed(reg_.y), reg_.acc);
      break;
    default:
      std::cout << "Unsupported STA variant: " << opcode << std::endl;
      return;
  }
}

void STX::ExecuteInternal(uint8_t opcode) {
  switch (opcode) {
    case 0x86:
      bus_.Write(ZeroPage(), reg_.x);
      break;
    case 0x96:
      bus_.Write(IndexedZeroPage(reg_.y), reg_.x);
      break;
    case 0x8E:
      bus_.Write(Absolute(), reg_.acc);
      break;
    default:
      std::cout << "Unsupported STX variant: " << opcode << std::endl;
      return;
  }
}

void STY::ExecuteInternal(uint8_t opcode) {
  switch (opcode) {
    case 0x84:
      bus_.Write(ZeroPage(), reg_.y);
      break;
    case 0x94:
      bus_.Write(IndexedZeroPage(reg_.x), reg_.y);
      break;
    case 0x8C:
      bus_.Write(Absolute(), reg_.y);
      break;
    default:
      std::cout << "Unsupported STY variant: " << opcode << std::endl;
      return;
  }
}
