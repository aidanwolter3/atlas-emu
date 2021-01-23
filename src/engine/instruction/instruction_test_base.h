#ifndef ENGINE_INSTRUCTION_INSTRUCTION_TEST_BASE_H_
#define ENGINE_INSTRUCTION_INSTRUCTION_TEST_BASE_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/engine/base/bus.h"
#include "src/engine/base/registers.h"
#include "src/engine/instruction/addressing.h"

class MockBus : public Bus {
 public:
  MOCK_METHOD2(Read, Peripheral::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Peripheral::Status(uint16_t address, uint8_t data));
  void RegisterPeripheral(Peripheral& peripheral, uint16_t start) {}
  uint16_t GetAddressLength() { return 0; }
};

class InstructionTestBase : public testing::Test {
 protected:
  int TimeInstruction(Instruction* instruction, Instruction::Mode mode,
                      Instruction::Operation operation) {
    int total_ticks = 0;
    bool finished = false;

    Instruction::Config config = {
        .mode = mode,
        .operation = operation,
        .instruction = instruction,
    };

    while (!finished) {
      total_ticks++;
      finished = addressing_.Execute(config, total_ticks);
    }
    return total_ticks;
  }

  MockBus bus_;
  Registers reg_;
  Addressing addressing_{bus_, reg_};
};

#endif  // ENGINE_INSTRUCTION_INSTRUCTION_TEST_BASE_H_
