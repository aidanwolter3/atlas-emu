#include "src/engine/cpu.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/engine/base/bus.h"
#include "src/engine/base/log.h"
#include "src/engine/base/registers.h"
#include "src/engine/instruction/addressing.h"
#include "src/engine/instruction/instruction.h"

namespace {

using testing::_;
using testing::DoAll;
using testing::Field;
using testing::Return;
using testing::SaveArg;
using testing::SetArgPointee;

const uint8_t kFakeOpcode = 0x12;

class MockBus : public Bus {
 public:
  MOCK_METHOD2(Read, Peripheral::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Peripheral::Status(uint16_t address, uint8_t data));
  void RegisterPeripheral(Peripheral& peripheral, uint16_t start) {}
  uint16_t GetAddressLength() { return 0; }
};

class MockInstruction : public Instruction {
 public:
  using Instruction::Instruction;
  MOCK_METHOD1(Execute, ExecuteResult(uint16_t operand));
};

void ExpectReadStartAddress(MockBus& bus, uint16_t address) {
  EXPECT_CALL(bus, Read(0xFFFC, _))
      .WillOnce(DoAll(SetArgPointee<1>(address & 0xFF),
                      Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus, Read(0xFFFD, _))
      .WillOnce(DoAll(SetArgPointee<1>((address >> 8) & 0xFF),
                      Return(Peripheral::Status::OK)));
}

class CpuTest : public testing::Test {
 public:
  void SetUp() override { Log::Init(Log::Level::ERROR); }
  void TearDown() override { Log::Reset(); }
};

TEST_F(CpuTest, RunUntilSegfault) {
  MockBus bus;
  Registers reg;

  // Prepare the Cpu
  ExpectReadStartAddress(bus, 0xBBAA);
  Cpu cpu(bus, reg);
  cpu.Reset();

  // Add a mock instruction
  MockInstruction mock_instruction(bus, reg);
  cpu.RegisterInstruction({
      .opcode = kFakeOpcode,
      .mode = Instruction::Mode::kImplied,
      .operation = Instruction::Operation::kNone,
      .instruction = &mock_instruction,
  });

  EXPECT_CALL(bus, Read(0xBBAA, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(kFakeOpcode), Return(Peripheral::Status::OK)));
  cpu.Tick();
  EXPECT_CALL(mock_instruction, Execute(_));
  cpu.Tick();
  EXPECT_EQ(0xBBAB, reg.pc);

  EXPECT_CALL(bus, Read(0xBBAB, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(kFakeOpcode), Return(Peripheral::Status::OK)));
  cpu.Tick();
  EXPECT_CALL(mock_instruction, Execute(_));
  cpu.Tick();
  EXPECT_EQ(0xBBAC, reg.pc);

  EXPECT_CALL(bus, Read(0xBBAC, _))
      .WillOnce(Return(Peripheral::Status::OUT_OF_BOUNDS));
  cpu.Tick();
  EXPECT_TRUE(Log::FoundLogWithLevel(Log::Level::ERROR));
  EXPECT_EQ(0xBBAC, reg.pc);
}

TEST_F(CpuTest, InitialStateOfStatusRegister) {
  MockBus bus;
  Registers reg;

  // Prepare the Cpu
  ExpectReadStartAddress(bus, 0xBBAA);
  Cpu cpu(bus, reg);
  cpu.Reset();

  EXPECT_FALSE(reg.status.test(Status::kCarry));
  EXPECT_FALSE(reg.status.test(Status::kZero));
  EXPECT_FALSE(reg.status.test(Status::kIntDisable));
  EXPECT_FALSE(reg.status.test(Status::kBCDMode));
  EXPECT_FALSE(reg.status.test(Status::kBrk));
  EXPECT_FALSE(reg.status.test(Status::kOverflow));
  EXPECT_FALSE(reg.status.test(Status::kSign));
}

}  // namespace
