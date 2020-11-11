#include "src/cpu.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/public/instruction.h"
#include "src/public/memory.h"
#include "src/public/registers.h"

namespace {

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

const uint8_t kFakeOpcode = 0x12;

class MockMemory : public Memory {
 public:
  MOCK_METHOD2(Read, Memory::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Memory::Status(uint16_t address, uint8_t data));
};

class MockInstruction : public Instruction {
 public:
  using Instruction::Instruction;
  MOCK_METHOD1(Execute, void(uint8_t opcode));
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
  auto mock_instruction = std::make_unique<MockInstruction>(mem, reg);
  MockInstruction* instruction_ptr = mock_instruction.get();

  ExpectReadStartAddress(mem, 0xBBAA);
  Cpu cpu(mem, reg);
  cpu.RegisterInstruction(std::move(mock_instruction), {kFakeOpcode});

  EXPECT_CALL(mem, Read(0xBBAA, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(kFakeOpcode), Return(Memory::Status::OK)));
  EXPECT_CALL(*instruction_ptr, Execute(kFakeOpcode));
  EXPECT_EQ(Cpu::Status::OK, cpu.Run());
  EXPECT_EQ(0xBBAB, reg.pc);

  EXPECT_CALL(mem, Read(0xBBAB, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(kFakeOpcode), Return(Memory::Status::OK)));
  EXPECT_CALL(*instruction_ptr, Execute(kFakeOpcode));
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

}  // namespace
