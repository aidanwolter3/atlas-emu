#ifndef ENGINE_INSTRUCTION_INSTRUCTION_TEST_BASE_H_
#define ENGINE_INSTRUCTION_INSTRUCTION_TEST_BASE_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/engine/base/bus.h"
#include "src/engine/base/registers.h"

class MockBus : public Bus {
 public:
  MOCK_METHOD2(Read, Peripheral::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Peripheral::Status(uint16_t address, uint8_t data));
  void RegisterPeripheral(Peripheral& peripheral, uint16_t start) {}
  uint16_t GetAddressLength() { return 0; }
};

class InstructionTestBase : public testing::Test {
 protected:
  MockBus bus_;
  Registers reg_;
};

#endif  // ENGINE_INSTRUCTION_INSTRUCTION_TEST_BASE_H_
