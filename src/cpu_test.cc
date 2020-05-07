#include "cpu.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "memory.h"

namespace {

using testing::_;
using testing::Return;

class MockMemory : public Memory {
 public:
  MOCK_METHOD2(Read, Memory::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Memory::Status(uint16_t address, uint8_t data));
};

TEST(CpuTest, RunUntilSegfault) {
  MockMemory mem;
  Cpu cpu(mem);

  EXPECT_CALL(mem, Read(0, _)).WillOnce(Return(Memory::Status::OK));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(1, cpu.GetPc());

  EXPECT_CALL(mem, Read(1, _)).WillOnce(Return(Memory::Status::OK));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(2, cpu.GetPc());

  EXPECT_CALL(mem, Read(2, _)).WillOnce(Return(Memory::Status::OUT_OF_BOUNDS));
  EXPECT_EQ(Cpu::Status::SEGFAULT, cpu.Run());
  EXPECT_EQ(2, cpu.GetPc());
}

}  // namespace
