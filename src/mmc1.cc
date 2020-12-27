#include "src/mmc1.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

const uint16_t kHeaderSize = 0x0010;
// A single PRG is only 0x4000 bytes.
const uint16_t kPRGSize = 0x4000;
// But we may have to PRGs, or the first PRG could be mirrored.
const uint16_t kMMC1Size = 0x8000;

std::string IntToHexString(int num) {
  std::stringstream ss;
  ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << num;
  return ss.str();
}

Ppu::MirroringMode MirroringModeFromControl(uint8_t control) {
  switch (control & 0x03) {
    case 0:
      return Ppu::MirroringMode::kOneScreenLower;
    case 1:
      return Ppu::MirroringMode::kOneScreenUpper;
    case 2:
      return Ppu::MirroringMode::kVertical;
    case 3:
      return Ppu::MirroringMode::kHorizontal;
    default:
      return Ppu::MirroringMode::kHorizontal;
  }
}

}  // namespace

class MMC1Impl::Header {
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
    return (prg_count_ <= 64);
  }

  int GetPrgCount() { return prg_count_; }
  int GetChrCount() { return chr_count_; }

 private:
  int prg_count_ = -1;
  int chr_count_ = -1;
};

MMC1Impl::MMC1Impl(Ppu& ppu, std::vector<uint8_t> data)
    : ppu_(ppu), header_(std::make_unique<Header>(data)) {
  if (!header_->IsValid()) {
    std::cout << "Header is invalid" << std::endl;
    return;
  }

  // Load the PRG.
  auto prg_begin = data.begin() + kHeaderSize;
  auto prg_end = prg_begin + (kPRGSize * header_->GetPrgCount());
  std::move(prg_begin, prg_end, std::back_inserter(prg_));
}

MMC1Impl::~MMC1Impl() = default;

void MMC1Impl::DumpRegisters() {
  std::cout << "-- mmc1 --" << std::endl;
  std::cout << "MMC1_CTRL=" << IntToHexString(control_) << std::endl;
  std::cout << "MMC1_CHR_0=" << IntToHexString(chr_bank_0_) << std::endl;
  std::cout << "MMC1_CHR_1=" << IntToHexString(chr_bank_1_) << std::endl;
  std::cout << "MMC1_PRG=" << IntToHexString(prg_bank_) << std::endl;
  std::cout << "----------" << std::endl;
}

Peripheral::Status MMC1Impl::Read(uint16_t address, uint8_t* byte) {
  if (!byte || !header_->IsValid() || address >= kMMC1Size) {
    return Peripheral::Status::OUT_OF_BOUNDS;
  }

  // If the PRG size is smaller than the offset, this means that we only have
  // one PRG block that is also mapped to the second PRG slot. Adjust the
  // offset in this situation to point to the data in the first slot.
  if (address >= prg_.size()) {
    address -= kPRGSize;
  }

  // Adjust the address to account for the block mapping.
  //
  // TODO: Implement other control modes.
  // Currently, we assume control mode 3:
  // * last bank (7) fixed at $c000
  // * first bank switchable and specified by |control_|
  int prg_bank_num;
  if (address < kPRGSize) {
    prg_bank_num = prg_bank_ & 0x0F;
  } else {
    prg_bank_num = header_->GetPrgCount() - 1;
  }

  // The PRG may be up to 128K in MMC1, but mapped into 16K of address range. We
  // need to use a uint32_t to index into the PRG.
  uint32_t prg_address = (address % kPRGSize) + (prg_bank_num * kPRGSize);

  *byte = prg_[prg_address];
  return Peripheral::Status::OK;
}

Peripheral::Status MMC1Impl::Write(uint16_t address, uint8_t byte) {
  if (address >= kMMC1Size) {
    return Peripheral::Status::OUT_OF_BOUNDS;
  }

  bool should_transfer_shift = false;

  // Reset.
  if (byte & 0xF0) {
    // The shift is initialized with a marker (1) to help identify when the
    // shift should be transfered to a register.
    shift_ = 0x10;
    // Set the control to mode 3.
    control_ |= 0x0C;
    // std::cout << "mmc1 reset" << std::endl;
    // std::cout << "mmc1 control=" << std::bitset<5>(control_) << std::endl;
    return Peripheral::Status::OK;
  }

  // Shift.
  else {
    // The shift is transfered to a register if we have moved in 5 bits (i.e.
    // the marker (1) is about to be shift out.
    should_transfer_shift = (shift_ & 0x01);
    // Shift bit-1 of |byte| into bit-5 of |shift_|.
    shift_ = shift_ >> 1;
    shift_ |= ((byte & 0x01) << 4);
  }

  // Transfer the shift to a register.
  if (should_transfer_shift) {
    uint8_t data = shift_ | (byte & 0x01) << 4;
    int register_num = (address >> 13);
    switch (register_num) {
      case 0:
        control_ = data;
        ppu_.SetMirroringMode(MirroringModeFromControl(control_));
        // std::cout << "mmc1 control=" << std::bitset<5>(control_)
        //           << std::endl;
        break;
      case 1:
        chr_bank_0_ = data;
        // std::cout << "mmc1 chr-bank1=" << std::bitset<5>(chr_bank_0_)
        //           << std::endl;
        break;
      case 2:
        chr_bank_1_ = data;
        // std::cout << "mmc1 chr-bank2=" << std::bitset<5>(chr_bank_1_)
        //           << std::endl;
        break;
      case 3:
        prg_bank_ = data;
        // std::cout << "mmc1 prg-bank=" << std::bitset<5>(prg_bank_)
        //           << std::endl;
        break;
      default:
        // std::cout << "Unexpected register: " << register_num << std::endl;
        break;
    }
    // Reset the shift.
    shift_ = 0x10;
  }

  return Peripheral::Status::OK;
}

uint16_t MMC1Impl::GetAddressLength() { return kMMC1Size; }
