#include "memory.h"

Memory::Memory(std::vector<uint8_t> data)
    : data_(std::move(data)) {}

Memory::~Memory() = default;

Memory::Status Memory::Read(uint16_t address, uint8_t* byte) {
  *byte = data_[address];
  return Status::OK;
}

Memory::Status Memory::Write(uint16_t address, uint8_t byte) {
  return Status::OK;
}
