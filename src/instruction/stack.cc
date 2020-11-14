#include "src/instruction/stack.h"

namespace {

const uint16_t kStackStartAddress = 0x100;

}  // namespace

void TXS::ExecuteInternal(uint8_t) { reg_.sp = reg_.x; }

void TSX::ExecuteInternal(uint8_t) { reg_.x = reg_.sp; }

void PHA::ExecuteInternal(uint8_t) {
  mem_.Write(kStackStartAddress + reg_.sp--, reg_.acc);
}

void PLA::ExecuteInternal(uint8_t) {
  mem_.Read(kStackStartAddress + ++reg_.sp, &reg_.acc);
}

void PHP::ExecuteInternal(uint8_t) {
  mem_.Write(kStackStartAddress + reg_.sp--, reg_.status.to_ulong());
}

void PLP::ExecuteInternal(uint8_t) {
  uint8_t byte_read;
  mem_.Read(kStackStartAddress + ++reg_.sp, &byte_read);
  reg_.status = byte_read;
}
