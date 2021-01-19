#ifndef ENGINE_INSTRUCTION_INSTRUCTION_H_
#define ENGINE_INSTRUCTION_INSTRUCTION_H_

#include <cstdint>
#include <string>

#include "src/engine/base/bus.h"
#include "src/engine/base/registers.h"

#define SET_LOG_NAME(n) \
  std::string GetLogName() override { return n; }

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

  struct ExecuteResult {
    // Data to be written in kReadWrite instructions.
    uint8_t data = 0;
    // Additional cycles to consume due to the instruction execution.
    int cycles = 0;
  };

  virtual ExecuteResult Execute(uint16_t operand) = 0;

  // Rather than overriding this method, Instructions should use the
  // SET_LOG_NAME macro in their header to provde the Instruction name to log.
  virtual std::string GetLogName() { return "TBD"; }

 protected:
  Bus& bus_;
  Registers& reg_;
};

#endif  // ENGINE_INSTRUCTION_INSTRUCTION_H_
