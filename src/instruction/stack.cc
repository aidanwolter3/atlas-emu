#include "src/instruction/stack.h"

#include "src/public/constants.h"

void PHA::ExecuteInternal(uint8_t) {
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.acc);
}

void PLA::ExecuteInternal(uint8_t) {
  bus_.Read(kStackStartAddress + ++reg_.sp, &reg_.acc);
}

void PHP::ExecuteInternal(uint8_t) {
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.status.to_ulong());
}

void PLP::ExecuteInternal(uint8_t) {
  uint8_t byte_read;
  bus_.Read(kStackStartAddress + ++reg_.sp, &byte_read);
  reg_.status = byte_read;
}
