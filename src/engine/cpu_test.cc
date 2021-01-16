#include "src/engine/cpu.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/engine/instruction/addressing.h"
#include "src/engine/instruction/instruction.h"
#include "src/engine/public/bus.h"
#include "src/engine/public/event_logger.h"
#include "src/engine/public/registers.h"

namespace {

using testing::_;
using testing::DoAll;
using testing::Field;
using testing::Return;
using testing::SaveArg;
using testing::SetArgPointee;

const uint8_t kFakeOpcode = 0x12;

class MockEventLogger : public EventLogger {
 public:
  MOCK_METHOD(void, LogEvent, (Event event), (override));
  MOCK_METHOD(void, PrintLogs, (), (override));
};

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
  MOCK_METHOD1(Execute, uint8_t(uint16_t operand));
};

void ExpectReadStartAddress(MockBus& bus, uint16_t address) {
  EXPECT_CALL(bus, Read(0xFFFC, _))
      .WillOnce(DoAll(SetArgPointee<1>(address & 0xFF),
                      Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus, Read(0xFFFD, _))
      .WillOnce(DoAll(SetArgPointee<1>((address >> 8) & 0xFF),
                      Return(Peripheral::Status::OK)));
}

TEST(CpuTest, RunUntilSegfault) {
  MockEventLogger event_logger;
  MockBus bus;
  Registers reg;

  // Prepare the Cpu
  ExpectReadStartAddress(bus, 0xBBAA);
  Cpu cpu(event_logger, bus, reg);
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
  EXPECT_CALL(event_logger, LogEvent(Field(&EventLogger::Event::type,
                                           EventLogger::EventType::kError)));
  cpu.Tick();
  EXPECT_EQ(0xBBAC, reg.pc);
}

TEST(CpuTest, InitialStateOfStatusRegister) {
  MockEventLogger event_logger;
  MockBus bus;
  Registers reg;

  // Prepare the Cpu
  ExpectReadStartAddress(bus, 0xBBAA);
  Cpu cpu(event_logger, bus, reg);
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
