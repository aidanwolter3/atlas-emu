#include "src/engine/joystick.h"

namespace {

// Space for addresses 4016/4017.
const uint16_t kJoystickSize = 2;

}  // namespace

Joystick::Joystick(Input& input) : input_(input) {}
Joystick::~Joystick() = default;

Peripheral::Status Joystick::Read(uint16_t address, uint8_t* byte) {
  // Expansions are not supported yet.
  if (address != 0x0000) {
    *byte = 0;
    return Peripheral::Status::OK;
  }

  // Convert the state to individual uint8_ts.
  Input::State state = input_.GetButtonState();
  bool state_bytes[8];
  state_bytes[0] = state.a;
  state_bytes[1] = state.b;
  state_bytes[2] = state.select;
  state_bytes[3] = state.start;
  state_bytes[4] = state.up;
  state_bytes[5] = state.down;
  state_bytes[6] = state.left;
  state_bytes[7] = state.right;

  // Return the correct button state.
  if (read_count_ < 8) {
    *byte = state_bytes[read_count_];
  } else {
    // The standard NES controller returns 0x01 after all buttons have been
    // read.
    *byte = 0x01;
  }
  read_count_++;
  return Peripheral::Status::OK;
}

Peripheral::Status Joystick::Write(uint16_t address, uint8_t byte) {
  if (address != 0x0000) {
    return Peripheral::Status::READ_ONLY;
  }

  // High to Low transition.
  if ((strobe_ & 0x01) && !(byte & 0x01)) {
    // std::cout << "Joystick: latch data" << std::endl;
    read_count_ = 0;
  }
  strobe_ = byte;
  return Peripheral::Status::OK;
}

uint16_t Joystick::GetAddressLength() { return kJoystickSize; }
