#ifndef ENGINE_INSTRUCTION_MISC_H_
#define ENGINE_INSTRUCTION_MISC_H_

#include "src/engine/instruction/instruction.h"
#include "src/engine/public/event_logger.h"

// No Operation
class NOP : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
               int cycle) override;
};

// Break (used for e2e tests)
class BRK : public Instruction2 {
 public:
  explicit BRK(Bus& bus, Registers& reg, EventLogger& event_logger);
  bool Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
               int cycle) override;

 private:
  EventLogger& event_logger_;
};

#endif  // ENGINE_INSTRUCTION_MISC_H_
