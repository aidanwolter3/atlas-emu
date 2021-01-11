#ifndef ENGINE_INSTRUCTION_ADDRESSING_H_
#define ENGINE_INSTRUCTION_ADDRESSING_H_

#include <cstdint>
#include <optional>

#include "src/engine/instruction/instruction.h"
#include "src/engine/public/bus.h"
#include "src/engine/public/registers.h"

class Addressing {
 public:
  Addressing(Bus& bus, Registers& reg);
  bool Execute(Instruction2::Config& config, int cycle);

 private:
  uint16_t Immediate();
  uint16_t ImmediateAddress();
  uint16_t ZeroPage();
  uint16_t Absolute();
  uint16_t Indirect();
  uint16_t IndirectX();
  uint16_t IndirectY();

  Bus& bus_;
  Registers& reg_;
  std::optional<uint16_t> address_;
  uint16_t operand_;
};

#endif  // ENGINE_INSTRUCTION_ADDRESSING_H_
