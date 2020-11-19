#include "src/storage.h"

#include <iostream>

namespace {

const uint16_t kHeaderSize = 0x0010;
// A single PRG is only 0x4000 bytes.
const uint16_t kPRGSize = 0x4000;
// But we may have to PRGs in storage, or the first PRG could be mirrored.
const uint16_t kStorageSize = 0x8000;

}  // namespace

class StorageImpl::Header {
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
    // TODO: support CHR banks.
    return (prg_count_ <= 64 && chr_count_ == 0);
  }

  int GetPrgCount() { return prg_count_; }
  int GetChrCount() { return chr_count_; }

 private:
  int prg_count_ = -1;
  int chr_count_ = -1;
};

StorageImpl::StorageImpl(std::vector<uint8_t> data)
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

StorageImpl::~StorageImpl() = default;

Peripheral::Status StorageImpl::Read(uint16_t address, uint8_t* byte) {
  if (!byte || !header_->IsValid() || address >= kStorageSize) {
    return Peripheral::Status::OUT_OF_BOUNDS;
  }

  // If the PRG size is smaller than the offset, this means that we only have
  // one PRG block that is also mapped to the second PRG slot. Adjust the
  // offset in this situation to point to the data in the first slot.
  if (address >= prg_.size()) {
    address -= kPRGSize;
  }

  *byte = prg_[address];
  return Peripheral::Status::OK;
}

Peripheral::Status StorageImpl::Write(uint16_t address, uint8_t byte) {
  return Peripheral::Status::READ_ONLY;
}

uint16_t StorageImpl::GetAddressLength() { return kStorageSize; }
