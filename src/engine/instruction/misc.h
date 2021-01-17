#ifndef ENGINE_INSTRUCTION_MISC_H_
#define ENGINE_INSTRUCTION_MISC_H_

#include "src/engine/base/event_logger.h"
#include "src/engine/instruction/instruction.h"

// No Operation
class NOP : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Break (used for e2e tests)
class BRK : public Instruction {
 public:
  explicit BRK(Bus& bus, Registers& reg, EventLogger& event_logger);
  uint8_t Execute(uint16_t operand) override;

 private:
  EventLogger& event_logger_;
};

#endif  // ENGINE_INSTRUCTION_MISC_H_
