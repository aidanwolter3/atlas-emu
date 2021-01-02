#ifndef ENGINE_INSTRUCTION_MISC_H_
#define ENGINE_INSTRUCTION_MISC_H_

#include "src/public/event_logger.h"
#include "src/public/instruction.h"

// No Operation
class NOP : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("NOP");
};

// Break (used for e2e tests)
class BRK : public Instruction {
 public:
  explicit BRK(Bus& bus, Registers& reg, EventLogger& event_logger);
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("BRK");

 private:
  EventLogger& event_logger_;
};

#endif  // ENGINE_INSTRUCTION_MISC_H_
