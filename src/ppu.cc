#include "src/ppu.h"

namespace {

const uint16_t kPpuSize = 8;

// Register offsets.
const uint16_t kPpuCtrl = 0;
const uint16_t kPpuMask = 1;
const uint16_t kPpuStatus = 2;
const uint16_t kOamAddr = 3;

}  // namespace

Ppu::Ppu() = default;
Ppu::~Ppu() = default;

Peripheral::Status Ppu::Read(uint16_t address, uint8_t* byte) {
  if (address == kPpuCtrl || address == kPpuMask || address == kOamAddr) {
    return Peripheral::Status::WRITE_ONLY;
  }
  // TODO: Implement.
  if (address == kPpuStatus) {
    // For now, we put a dummy value in to get past the Vblank checks at the
    // start of ROMs.
    *byte = 0x80;
  } else {
    *byte = 0x00;
  }
  return Peripheral::Status::OK;
}

Peripheral::Status Ppu::Write(uint16_t address, uint8_t byte) {
  if (address == kPpuStatus) {
    return Peripheral::Status::READ_ONLY;
  }
  // TODO: Implement.
  return Peripheral::Status::OK;
}

uint16_t Ppu::GetAddressLength() { return kPpuSize; }
