#include "src/engine/instruction/misc.h"

#include <string>

uint8_t NOP::Execute(uint8_t opcode, uint16_t operand) { return 0; }

BRK::BRK(Bus& bus, Registers& reg, EventLogger& event_logger)
    : Instruction2(bus, reg), event_logger_(event_logger) {}

uint8_t BRK::Execute(uint8_t opcode, uint16_t operand) {
  // The accumulator holds the test result.
  // 0 is success; everything else is failure.
  bool passed = reg_.acc == 0;
  std::string event_name = passed ? "Test passed" : "Test failed";
  EventLogger::EventType event_type = passed
                                          ? EventLogger::EventType::kTestPassed
                                          : EventLogger::EventType::kTestFailed;
  event_logger_.LogEvent({.type = event_type, .name = event_name});

  // Note: We do not care about having cycle accuracy here, because this ends
  // the program.
  return 0;
}
