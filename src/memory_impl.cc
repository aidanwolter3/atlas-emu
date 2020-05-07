#include "memory_impl.h"

MemoryImpl::MemoryImpl(std::vector<uint8_t> data) : data_(std::move(data)) {}

Memory::Status MemoryImpl::Read(uint16_t address, uint8_t* byte) {
  if (!byte || address > (data_.size() - 1)) {
    return Memory::Status::OUT_OF_BOUNDS;
  }

  *byte = data_[address];
  return Memory::Status::OK;
}

Memory::Status MemoryImpl::Write(uint16_t address, uint8_t byte) {
  return Memory::Status::OK;
}
