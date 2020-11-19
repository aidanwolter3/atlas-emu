#include "src/memory.h"

namespace {

// The RAM is only 0x0800 bytes.
const uint16_t kRAMSize = 0x0800;
// But is mirrored to 0x2000 of memory space.
const uint16_t kMemorySize = 0x2000;

}  // namespace

MemoryImpl::MemoryImpl() : data_(kRAMSize, 0) {}
MemoryImpl::~MemoryImpl() = default;

MemoryImpl::Status MemoryImpl::Read(uint16_t address, uint8_t* byte) {
  if (!byte || address >= kMemorySize) {
    return MemoryImpl::Status::OUT_OF_BOUNDS;
  }

  *byte = data_[address % kRAMSize];
  return MemoryImpl::Status::OK;
}

MemoryImpl::Status MemoryImpl::Write(uint16_t address, uint8_t byte) {
  if (address >= kMemorySize) {
    return MemoryImpl::Status::OUT_OF_BOUNDS;
  }

  data_[address % kRAMSize] = byte;
  return MemoryImpl::Status::OK;
}

uint16_t MemoryImpl::GetAddressLength() { return kMemorySize; }
