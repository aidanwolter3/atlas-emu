#ifndef ENGINE_JOYSTICK_H_
#define ENGINE_JOYSTICK_H_

#include "src/engine/base/bus.h"
#include "src/input/input.h"

class Joystick : public Peripheral {
 public:
  Joystick(Input& input);
  ~Joystick() override;

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;

 private:
  Input& input_;
  uint8_t strobe_ = 0;
  int read_count_ = 0;
};

#endif  // ENGINE_JOYSTICK_H_
