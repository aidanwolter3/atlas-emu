#ifndef ENGINE_INSTRUCTION_ADDRESSING_H_
#define ENGINE_INSTRUCTION_ADDRESSING_H_

#include <cstdint>

#include "src/engine/public/bus.h"
#include "src/engine/public/registers.h"

class Addressing {
 public:
  enum class Mode {
    kImplied,
    kImmediate,
    kImmediateAddress,
    kZeroPage,
    kAbsolute,
    kIndirect,
  };

  Addressing(Bus& bus, Registers& reg);
  bool FetchOperand(Mode mode, int cycle, uint16_t* operand);

 private:
  bool Immediate(int cycle, uint16_t* operand);
  bool ImmediateAddress(int cycle, uint16_t* operand);
  bool ZeroPage(int cycle, uint16_t* operand);
  bool Absolute(int cycle, uint16_t* operand);
  bool Indirect(int cycle, uint16_t* operand);

  Bus& bus_;
  Registers& reg_;
};

#endif  // ENGINE_INSTRUCTION_ADDRESSING_H_
