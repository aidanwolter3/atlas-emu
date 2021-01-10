#ifndef ENGINE_INSTRUCTION_ADDRESSING_MODE_H_
#define ENGINE_INSTRUCTION_ADDRESSING_MODE_H_

#include <cstdint>

#include "src/engine/public/bus.h"
#include "src/engine/public/registers.h"

class AddressingMode {
 public:
  AddressingMode(Bus& bus, Registers& reg) : bus_(bus), reg_(reg) {}
  virtual ~AddressingMode() {}
  virtual bool FetchOperand(int cycle, uint16_t* operand) = 0;

 protected:
  Bus& bus_;
  Registers& reg_;
};

class Immediate : public AddressingMode {
 public:
  using AddressingMode::AddressingMode;
  bool FetchOperand(int cycle, uint16_t* operand) override;
};

class Absolute : public AddressingMode {
 public:
  using AddressingMode::AddressingMode;
  bool FetchOperand(int cycle, uint16_t* operand) override;
};

class Indirect : public AddressingMode {
 public:
  using AddressingMode::AddressingMode;
  bool FetchOperand(int cycle, uint16_t* operand) override;
};

#endif  // ENGINE_INSTRUCTION_ADDRESSING_MODE_H_
