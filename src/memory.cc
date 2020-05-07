#include "memory.h"

Memory::Memory(std::vector<uint8_t> data)
    : data_(std::move(data)) {}

Memory::~Memory() = default;

uint8_t Memory::Read(uint16_t address) {
  return data_[address];
}

uint8_t Memory::Write(uint16_t address) {
  return 0;
}
