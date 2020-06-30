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

void ExpectReadStartAddress(MockMemory& mem, uint16_t address) {
  EXPECT_CALL(mem, Read(0xFFFC, _)).WillOnce(DoAll(
        SetArgPointee<1>(address & 0xFF), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(0xFFFD, _)).WillOnce(DoAll(
        SetArgPointee<1>((address >> 8) & 0xFF), Return(Memory::Status::OK)));
}

TEST(CpuTest, RunUntilSegfault) {
  MockMemory mem;

  ExpectReadStartAddress(mem, 0xBBAA);
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

TEST(CpuTest, InitialStateOfStatusRegister) {
  MockMemory mem;

  ExpectReadStartAddress(mem, 0xBBAA);
  Cpu cpu(mem);

  EXPECT_FALSE(cpu.GetStatusRegister().carry);
  EXPECT_FALSE(cpu.GetStatusRegister().zero);
  EXPECT_FALSE(cpu.GetStatusRegister().int_disable);
  EXPECT_FALSE(cpu.GetStatusRegister().bcd_mode);
  EXPECT_FALSE(cpu.GetStatusRegister().brk);
  EXPECT_FALSE(cpu.GetStatusRegister().overflow);
  EXPECT_FALSE(cpu.GetStatusRegister().sign);
}

}  // namespace
