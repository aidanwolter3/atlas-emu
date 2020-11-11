#include "src/instruction/status.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/public/memory.h"
#include "src/public/registers.h"

namespace {

class MockMemory : public Memory {
 public:
  MOCK_METHOD2(Read, Memory::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Memory::Status(uint16_t address, uint8_t data));
};

TEST(StatusTest, SetInterruptDisable) {
  MockMemory mem;
  Registers reg;
  SEI sei(mem, reg);

  reg.status.set(Status::kIntDisable);
  sei.Execute(0x78);
  EXPECT_FALSE(reg.status.test(Status::kZero));
}

TEST(StatusTest, ClearDecimalMode) {
  MockMemory mem;
  Registers reg;
  CLD cld(mem, reg);

  reg.status.set(Status::kBCDMode);
  cld.Execute(0xD8);
  EXPECT_FALSE(reg.status.test(Status::kBCDMode));
}

}  // namespace
