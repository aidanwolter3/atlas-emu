#include "src/instruction/misc.h"

#include <string>

void NOP::ExecuteInternal(uint8_t opcode) {}

BRK::BRK(Bus& bus, Registers& reg, EventLogger& event_logger)
    : Instruction(bus, reg), event_logger_(event_logger) {}

void BRK::ExecuteInternal(uint8_t opcode) {
  // The accumulator holds the test result.
  // 0 is success; everything else is failure.
  bool passed = reg_.acc == 0;
  std::string event_name = passed ? "Test passed" : "Test failed";
  EventLogger::EventType event_type = passed
                                          ? EventLogger::EventType::kTestPassed
                                          : EventLogger::EventType::kTestFailed;
  event_logger_.LogEvent({.type = event_type, .name = event_name});
}
