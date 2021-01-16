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
  bool Execute(Instruction::Config& config, int cycle);

 private:
  struct Result {
    uint16_t data;
    int cycles;
  };

  Result Immediate(Instruction::Operation operation);
  Result ImmediateAddress(Instruction::Operation operation);
  Result ZeroPage(Instruction::Operation operation);
  Result ZeroPageX(Instruction::Operation operation);
  Result ZeroPageY(Instruction::Operation operation);
  Result Absolute(Instruction::Operation operation);
  Result AbsoluteX(Instruction::Operation operation);
  Result AbsoluteY(Instruction::Operation operation);
  Result Indirect(Instruction::Operation operation);
  Result IndirectX(Instruction::Operation operation);
  Result IndirectY(Instruction::Operation operation);

  Bus& bus_;
  Registers& reg_;
  std::optional<uint16_t> address_;
  std::optional<uint16_t> operand_;
  int cycles_;
};

#endif  // ENGINE_INSTRUCTION_ADDRESSING_H_
