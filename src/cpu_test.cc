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

TEST(CpuTest, NOP) {
  MockMemory mem;

  ExpectReadStartAddress(mem, 0xBBAA);
  Cpu cpu(mem);

  EXPECT_CALL(mem, Read(0xBBAA, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xEA), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(0xBBAB, cpu.GetPc());
}

TEST(CpuTest, SEI) {
  MockMemory mem;

  ExpectReadStartAddress(mem, 0xBBAA);
  Cpu cpu(mem);

  EXPECT_CALL(mem, Read(0xBBAA, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x78), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(0xBBAB, cpu.GetPc());
  EXPECT_TRUE(cpu.GetStatusRegister().int_disable);
}

TEST(CpuTest, LDA) {
  MockMemory mem;

  uint16_t pc = 0xBBAA;
  ExpectReadStartAddress(mem, pc);
  Cpu cpu(mem);

  // Immediate addressing
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xA9), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(pc, cpu.GetPc());
  EXPECT_EQ(0x12, cpu.GetAcc());
  EXPECT_FALSE(cpu.GetStatusRegister().zero);
  EXPECT_FALSE(cpu.GetStatusRegister().sign);

  // Zero-page addressing
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xA5), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(0x12, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(pc, cpu.GetPc());
  EXPECT_EQ(0x34, cpu.GetAcc());

  // Status register (zero positive)
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xA9), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x00), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(pc, cpu.GetPc());
  EXPECT_EQ(0x00, cpu.GetAcc());
  EXPECT_TRUE(cpu.GetStatusRegister().zero);
  EXPECT_FALSE(cpu.GetStatusRegister().sign);

  // Status register (non-zero negative)
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xA9), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xFF), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(pc, cpu.GetPc());
  EXPECT_EQ(0xFF, cpu.GetAcc());
  EXPECT_FALSE(cpu.GetStatusRegister().zero);
  EXPECT_TRUE(cpu.GetStatusRegister().sign);
}

}  // namespace
