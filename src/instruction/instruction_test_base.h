#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/public/bus.h"
#include "src/public/registers.h"

class MockMemory : public Peripheral {
 public:
  MOCK_METHOD2(Read, Peripheral::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Peripheral::Status(uint16_t address, uint8_t data));
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

  MockMemory mem_;
  Registers reg_;
};
