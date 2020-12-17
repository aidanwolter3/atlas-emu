#include "src/ppu.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

const uint64_t kPpuPeriod = 1.0 / 0.00000006;
const uint16_t kPpuSize = 0x2000;

// Register offsets.
const uint16_t kPpuCtrl = 0;
const uint16_t kPpuMask = 1;
const uint16_t kPpuStatus = 2;
const uint16_t kOamAddr = 3;
const uint16_t kPpuScroll = 5;
const uint16_t kPpuAddress = 6;
const uint16_t kPpuData = 7;

// TODO: Move this to a shared utility class.
std::string IntToHexString(int num) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << std::hex << num;
  return ss.str();
}

}  // namespace

Ppu::Ppu(Clock& clock, Cpu& cpu) : cpu_(cpu), vram_(0x4000, 0) {
  clock.RegisterTimerObserver(this, kPpuPeriod);
}

Ppu::~Ppu() = default;

Peripheral::Status Ppu::Read(uint16_t address, uint8_t* byte) {
  address = address % 0x08;

  switch (address) {
    case kPpuCtrl:
    case kPpuMask:
    case kOamAddr:
      return Peripheral::Status::WRITE_ONLY;
    case kPpuStatus:
      // For now, we put a dummy value in to get past the Vblank checks at the
      // start of ROMs.
      *byte = 0x80 | (last_write_value_ & 0x1F);
      break;
    case kPpuData:
      // The memory range between 0x3000-0x3EFF are mirrors of 0x2000-0x2EFF.
      uint16_t vram_address = address_;
      if (vram_address >= 0x3000 && vram_address <= 0x3EFF) {
        vram_address -= 0x1000;
      }

      if (vram_address > vram_.size() - 1) {
        std::cout << "ppu error: Address is too large!" << std::endl;
        return Peripheral::Status::OUT_OF_BOUNDS;
      }

      *byte = vram_[vram_address];
      std::cout << "ppu read data: " << IntToHexString(vram_address) << "="
                << IntToHexString(*byte) << std::endl;
      address_ += (ctrl_ & 0x04) ? 32 : 1;
      break;
  }
  std::cout << "ppu read: " << IntToHexString(address) << "="
            << IntToHexString(*byte) << std::endl;
  return Peripheral::Status::OK;
}

Peripheral::Status Ppu::Write(uint16_t address, uint8_t byte) {
  last_write_value_ = byte;
  std::cout << "ppu write: " << IntToHexString(address) << "="
            << IntToHexString(byte) << std::endl;

  // We only have 8 registers.
  address = address % 0x08;

  switch (address) {
    case kPpuStatus:
      return Peripheral::Status::READ_ONLY;
    case kPpuCtrl:
      ctrl_ = byte;
      break;
    case kPpuMask:
      mask_ = byte;
      break;
    case kOamAddr:
      oam_address_ = byte;
      break;
    case kPpuScroll:
      // The first byte written is the X-scroll and the second byte is Y-scroll.
      // After that it continues alternating back and forth.
      if (scroll_write_to_x_) {
        scroll_x_ = byte;
      } else {
        scroll_y_ = byte;
      }
      scroll_write_to_x_ = !scroll_write_to_x_;
      break;
    case kPpuAddress:
      // The first byte written is the upper byte of the address, and the second
      // byte is the lower byte of the address. After that it continues
      // alternating back and forth.
      if (address_write_to_upper_) {
        address_ = (byte << 8);
      } else {
        address_ = (address_ & 0xFF00) + byte;
      }
      address_write_to_upper_ = !address_write_to_upper_;
      break;
    case kPpuData:
      // The memory range between 0x3000-0x3EFF are mirrors of 0x2000-0x2EFF.
      uint16_t vram_address = address_;
      if (vram_address >= 0x3000 && vram_address <= 0x3EFF) {
        vram_address -= 0x1000;
      }

      if (vram_address > vram_.size() - 1) {
        std::cout << "ppu error: Address is too large!" << std::endl;
        return Peripheral::Status::OUT_OF_BOUNDS;
      }

      std::cout << "ppu write data: " << IntToHexString(vram_address) << "="
                << IntToHexString(byte) << std::endl;
      vram_[vram_address] = byte;
      address_ += (ctrl_ & 0x04) ? 32 : 1;
      break;
  }
  return Peripheral::Status::OK;
}

uint16_t Ppu::GetAddressLength() { return kPpuSize; }

void Ppu::OnTimerCalled() {
  // Trigger the NMI if the control bit is set.
  if (ctrl_ & 0x80) {
    cpu_.NMI();
  }

  // Dump the nametable.
  // TODO: Suppress log output from this file based on command-line flag.
  std::cout << "NAMETABLE-1" << std::endl;
  for (int i = 0; i < 30; ++i) {
    for (int j = 0; j < 32; ++j) {
      int index = i * 32 + j + 0x2000;
      std::cout << " " << IntToHexString(vram_[index]);
    }
    std::cout << std::endl;
  }
}
