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
  struct Result {
    uint16_t data;
    int cycles;
  };

  Result Immediate(Instruction2::Operation operation);
  Result ImmediateAddress(Instruction2::Operation operation);
  Result ZeroPage(Instruction2::Operation operation);
  Result ZeroPageX(Instruction2::Operation operation);
  Result ZeroPageY(Instruction2::Operation operation);
  Result Absolute(Instruction2::Operation operation);
  Result AbsoluteX(Instruction2::Operation operation);
  Result AbsoluteY(Instruction2::Operation operation);
  Result Indirect(Instruction2::Operation operation);
  Result IndirectX(Instruction2::Operation operation);
  Result IndirectY(Instruction2::Operation operation);

  Bus& bus_;
  Registers& reg_;
  std::optional<uint16_t> address_;
  std::optional<uint16_t> operand_;
  int cycles_;
};

#endif  // ENGINE_INSTRUCTION_ADDRESSING_H_
