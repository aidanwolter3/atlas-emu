#include "src/cpu.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/public/bus.h"
#include "src/public/clock.h"
#include "src/public/event_logger.h"
#include "src/public/instruction.h"
#include "src/public/registers.h"

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

class MockClock : public Clock {
 public:
  MOCK_METHOD(void, RegisterTimerObserver,
              (TimerObserver * observer, uint64_t period_ns), (override));
  MOCK_METHOD(void, RunUntilTimer, (), (override));
  MOCK_METHOD(void, RunUntilTime, (uint64_t time_ns), (override));
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
  MOCK_METHOD1(ExecuteInternal, void(uint8_t opcode));
  SET_LOG_NAME("mock");
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
  MockClock clock;
  MockBus bus;
  Registers reg;

  // Prepare the Cpu
  Clock::TimerObserver* timer_observer = nullptr;
  EXPECT_CALL(clock, RegisterTimerObserver(_, _))
      .WillOnce(SaveArg<0>(&timer_observer));
  ExpectReadStartAddress(bus, 0xBBAA);
  Cpu cpu(event_logger, clock, bus, reg);
  ASSERT_NE(nullptr, timer_observer);

  // Add a mock instruction
  auto mock_instruction = std::make_unique<MockInstruction>(bus, reg);
  MockInstruction* instruction_ptr = mock_instruction.get();
  cpu.RegisterInstruction(std::move(mock_instruction), {kFakeOpcode});

  EXPECT_CALL(bus, Read(0xBBAA, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(kFakeOpcode), Return(Peripheral::Status::OK)));
  EXPECT_CALL(*instruction_ptr, ExecuteInternal(kFakeOpcode));
  timer_observer->OnTimerCalled();
  EXPECT_EQ(0xBBAB, reg.pc);

  EXPECT_CALL(bus, Read(0xBBAB, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(kFakeOpcode), Return(Peripheral::Status::OK)));
  EXPECT_CALL(*instruction_ptr, ExecuteInternal(kFakeOpcode));
  timer_observer->OnTimerCalled();
  EXPECT_EQ(0xBBAC, reg.pc);

  EXPECT_CALL(bus, Read(0xBBAC, _))
      .WillOnce(Return(Peripheral::Status::OUT_OF_BOUNDS));
  EXPECT_CALL(event_logger, LogEvent(Field(&EventLogger::Event::type,
                                           EventLogger::EventType::kError)));
  timer_observer->OnTimerCalled();
  EXPECT_EQ(0xBBAD, reg.pc);
}

TEST(CpuTest, InitialStateOfStatusRegister) {
  MockEventLogger event_logger;
  MockClock clock;
  MockBus bus;
  Registers reg;

  // Prepare the Cpu
  ExpectReadStartAddress(bus, 0xBBAA);
  Cpu cpu(event_logger, clock, bus, reg);

  EXPECT_FALSE(reg.status.test(Status::kCarry));
  EXPECT_FALSE(reg.status.test(Status::kZero));
  EXPECT_FALSE(reg.status.test(Status::kIntDisable));
  EXPECT_FALSE(reg.status.test(Status::kBCDMode));
  EXPECT_FALSE(reg.status.test(Status::kBrk));
  EXPECT_FALSE(reg.status.test(Status::kOverflow));
  EXPECT_FALSE(reg.status.test(Status::kSign));
}

}  // namespace
