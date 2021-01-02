#include "src/engine/memory.h"

MemoryImpl::MemoryImpl(uint16_t size, int mirror_count)
    : size_(size), mirror_count_(mirror_count), data_(size * mirror_count, 0) {}

MemoryImpl::~MemoryImpl() = default;

MemoryImpl::Status MemoryImpl::Read(uint16_t address, uint8_t* byte) {
  if (!byte || address >= (size_ * mirror_count_)) {
    return MemoryImpl::Status::OUT_OF_BOUNDS;
  }

  *byte = data_[address % size_];
  return MemoryImpl::Status::OK;
}

MemoryImpl::Status MemoryImpl::Write(uint16_t address, uint8_t byte) {
  if (address >= (size_ * mirror_count_)) {
    return MemoryImpl::Status::OUT_OF_BOUNDS;
  }

  data_[address % size_] = byte;
  return MemoryImpl::Status::OK;
}

uint16_t MemoryImpl::GetAddressLength() { return size_ * mirror_count_; }
