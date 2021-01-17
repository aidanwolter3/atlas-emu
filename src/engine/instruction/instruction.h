#ifndef ENGINE_INSTRUCTION_INSTRUCTION_H_
#define ENGINE_INSTRUCTION_INSTRUCTION_H_

#include <cstdint>

#include "src/engine/base/bus.h"
#include "src/engine/base/registers.h"

class Instruction {
 public:
  enum class Mode {
    kImplied,
    kImmediate,
    kImmediateAddress,
    kZeroPage,
    kZeroPageX,
    kZeroPageY,
    kAbsolute,
    kAbsoluteX,
    kAbsoluteY,
    kIndirect,
    kIndirectX,
    kIndirectY,
  };

  enum class Operation {
    kNone,
    kRead,
    kWrite,
    kReadWrite,
  };

  struct Config {
    uint8_t opcode;
    Mode mode;
    Operation operation;
    Instruction* instruction;
  };

  Instruction(Bus& bus, Registers& reg) : bus_(bus), reg_(reg) {}
  virtual ~Instruction() {}
  virtual uint8_t Execute(uint16_t operand) = 0;

 protected:
  Bus& bus_;
  Registers& reg_;
};

#endif  // ENGINE_INSTRUCTION_INSTRUCTION_H_
