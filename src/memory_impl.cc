#include "src/memory_impl.h"

#include <algorithm>
#include <iostream>

namespace {

const uint16_t kHeaderSize = 0x0010;
const uint16_t kRAMSize = 0x0800;
const uint16_t kRAMEndAddress = 0x2000;
const uint16_t kPRGSize = 0x4000;
const uint16_t kPRGStartAddress = 0x8000;

}  // namespace

class MemoryImpl::Header {
 public:
  Header(const std::vector<uint8_t>& data) {
    if (data.size() < 16) {
      return;
    }
    prg_count_ = data[4];
    chr_count_ = data[5];
  }
  ~Header() = default;

  bool IsValid() {
    // TODO: support CHR memory banks.
    return (prg_count_ <= 64 && chr_count_ == 0);
  }

  int GetPrgCount() { return prg_count_; }
  int GetChrCount() { return chr_count_; }

 private:
  int prg_count_ = -1;
  int chr_count_ = -1;
};

MemoryImpl::MemoryImpl(std::vector<uint8_t> data)
    : header_(std::make_unique<Header>(data)) {
  if (!header_->IsValid()) {
    std::cout << "Header is invalid" << std::endl;
    return;
  }

  // Load the PRG.
  auto prg_begin = data.begin() + kHeaderSize;
  auto prg_end = prg_begin + (kPRGSize * header_->GetPrgCount());
  std::move(prg_begin, prg_end, std::back_inserter(prg_));
}

MemoryImpl::~MemoryImpl() = default;

Memory::Status MemoryImpl::Read(uint16_t address, uint8_t* byte) {
  if (!byte || !header_->IsValid()) {
    return Memory::Status::OUT_OF_BOUNDS;
  }

  // Address falls within RAM.
  if (address < kRAMEndAddress) {
    *byte = prg_[address % kRAMSize];
    return Memory::Status::OK;
  }

  // Address falls within the PRG address range.
  if (address >= kPRGStartAddress) {
    uint16_t prg_offset = address - kPRGStartAddress;

    // If the PRG size is smaller than the offset, this means that we only have
    // one PRG block that is also mapped to the second PRG slot. Adjust the
    // offset in this situation to point to the data in the first slot.
    if (prg_.size() <= prg_offset) {
      prg_offset -= kPRGSize;
    }

    *byte = prg_[prg_offset];
    return Memory::Status::OK;
  }

  return Memory::Status::OUT_OF_BOUNDS;
}

Memory::Status MemoryImpl::Write(uint16_t address, uint8_t byte) {
  if (!header_->IsValid()) {
    return Memory::Status::OUT_OF_BOUNDS;
  }

  // Address falls within RAM.
  if (address < kRAMEndAddress) {
    prg_[address % kRAMSize] = byte;
    return Memory::Status::OK;
  }

  return Memory::Status::OUT_OF_BOUNDS;
}
