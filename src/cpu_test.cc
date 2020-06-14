#include "cpu.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "memory.h"

namespace {

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

class MockMemory : public Memory {
 public:
  MOCK_METHOD2(Read, Memory::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Memory::Status(uint16_t address, uint8_t data));
};

TEST(CpuTest, RunUntilSegfault) {
  MockMemory mem;

  // The starting address will be read first.
  EXPECT_CALL(mem, Read(0xFFFA, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAA), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(0xFFFB, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xBB), Return(Memory::Status::OK)));
  Cpu cpu(mem);

  EXPECT_CALL(mem, Read(0xBBAA, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xEA), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(0xBBAB, cpu.GetPc());

  EXPECT_CALL(mem, Read(0xBBAB, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xEA), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(0xBBAC, cpu.GetPc());

  EXPECT_CALL(mem, Read(0xBBAC, _))
      .WillOnce(Return(Memory::Status::OUT_OF_BOUNDS));
  EXPECT_EQ(Cpu::Status::SEGFAULT, cpu.Run());
  EXPECT_EQ(0xBBAC, cpu.GetPc());
}

}  // namespace
