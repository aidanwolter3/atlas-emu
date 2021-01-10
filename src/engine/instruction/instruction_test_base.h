#ifndef ENGINE_INSTRUCTION_INSTRUCTION_TEST_BASE_H_
#define ENGINE_INSTRUCTION_INSTRUCTION_TEST_BASE_H_

#include <cstdint>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/engine/instruction/addressing.h"
#include "src/engine/instruction/instruction.h"
#include "src/engine/public/bus.h"
#include "src/engine/public/registers.h"

class MockBus : public Bus {
 public:
  MOCK_METHOD2(Read, Peripheral::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Peripheral::Status(uint16_t address, uint8_t data));
  void RegisterPeripheral(Peripheral& peripheral, uint16_t start) {}
  uint16_t GetAddressLength() { return 0; }
};

class Instruction2TestBase : public testing::Test {
 protected:
  // Executes an instruction until it completes, then returns how many cycles
  // were taken. By default, we assume implied addressing with no operand.
  int ExecuteUntilComplete(Instruction2* ins,
                           Addressing::Mode mode = Addressing::Mode::kImplied,
                           uint16_t operand = 0);

  MockBus bus_;
  Registers reg_;
};

class InstructionTestBase : public testing::Test {
 protected:
  uint16_t ExpectImmediate();
  void ExpectPostImmediate();
  uint16_t ExpectZeroPage();
  void ExpectPostZeroPage();
  uint16_t ExpectIndexedZeroPage(uint8_t& index);
  void ExpectPostIndexedZeroPage();
  uint16_t ExpectAbsolute();
  void ExpectPostAbsolute();
  uint16_t ExpectIndexedAbsolute(uint8_t& index);
  void ExpectPostIndexedAbsolute();
  uint16_t ExpectIndexedIndirect(uint8_t& index);
  void ExpectPostIndexedIndirect();
  uint16_t ExpectIndirectIndexed(uint8_t& index);
  void ExpectPostIndirectIndexed();

  MockBus bus_;
  Registers reg_;
};

#endif  // ENGINE_INSTRUCTION_INSTRUCTION_TEST_BASE_H_
