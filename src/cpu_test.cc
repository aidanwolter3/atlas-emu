#include "src/cpu.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/memory.h"
#include "src/registers.h"

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
  Registers reg;

  ExpectReadStartAddress(mem, 0xBBAA);
  Cpu cpu(mem, reg);

  EXPECT_CALL(mem, Read(0xBBAA, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xEA), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(0xBBAB, reg.pc);

  EXPECT_CALL(mem, Read(0xBBAB, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xEA), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(0xBBAC, reg.pc);

  EXPECT_CALL(mem, Read(0xBBAC, _))
      .WillOnce(Return(Memory::Status::OUT_OF_BOUNDS));
  EXPECT_EQ(Cpu::Status::SEGFAULT, cpu.Run());
  EXPECT_EQ(0xBBAC, reg.pc);
}

TEST(CpuTest, InitialStateOfStatusRegister) {
  MockMemory mem;
  Registers reg;

  ExpectReadStartAddress(mem, 0xBBAA);
  Cpu cpu(mem, reg);

  EXPECT_FALSE(reg.status.test(Status::kCarry));
  EXPECT_FALSE(reg.status.test(Status::kZero));
  EXPECT_FALSE(reg.status.test(Status::kIntDisable));
  EXPECT_FALSE(reg.status.test(Status::kBCDMode));
  EXPECT_FALSE(reg.status.test(Status::kBrk));
  EXPECT_FALSE(reg.status.test(Status::kOverflow));
  EXPECT_FALSE(reg.status.test(Status::kSign));
}

TEST(CpuTest, NOP) {
  MockMemory mem;
  Registers reg;

  ExpectReadStartAddress(mem, 0xBBAA);
  Cpu cpu(mem, reg);

  EXPECT_CALL(mem, Read(0xBBAA, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xEA), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(0xBBAB, reg.pc);
}

TEST(CpuTest, SEI) {
  MockMemory mem;
  Registers reg;

  ExpectReadStartAddress(mem, 0xBBAA);
  Cpu cpu(mem, reg);

  EXPECT_CALL(mem, Read(0xBBAA, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x78), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(0xBBAB, reg.pc);
  EXPECT_TRUE(reg.status.test(Status::kIntDisable));
}

TEST(CpuTest, LDA) {
  MockMemory mem;
  Registers reg;

  uint16_t pc = 0xBBAA;
  ExpectReadStartAddress(mem, pc);
  Cpu cpu(mem, reg);

  // Immediate addressing
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xA9), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(pc, reg.pc);
  EXPECT_EQ(0x12, reg.acc);
  EXPECT_FALSE(reg.status.test(Status::kZero));
  EXPECT_FALSE(reg.status.test(Status::kSign));

  // Zero-page addressing
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xA5), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(0x12, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(pc, reg.pc);
  EXPECT_EQ(0x34, reg.acc);

  // Status register (zero positive)
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xA9), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x00), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(pc, reg.pc);
  EXPECT_EQ(0x00, reg.acc);
  EXPECT_TRUE(reg.status.test(Status::kZero));
  EXPECT_FALSE(reg.status.test(Status::kSign));

  // Status register (non-zero negative)
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xA9), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(pc++, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xFF), Return(Memory::Status::OK)));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(pc, reg.pc);
  EXPECT_EQ(0xFF, reg.acc);
  EXPECT_FALSE(reg.status.test(Status::kZero));
  EXPECT_TRUE(reg.status.test(Status::kSign));
}

}  // namespace
