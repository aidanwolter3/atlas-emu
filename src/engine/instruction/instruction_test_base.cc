#include "src/engine/instruction/instruction_test_base.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

int Instruction2TestBase::ExecuteUntilComplete(Instruction2* ins,
                                               Instruction2::Mode mode,
                                               uint16_t operand) {
  int cycles = 0;
  std::optional<uint8_t> finished = std::nullopt;
  while (!finished) {
    cycles++;
    finished = ins->Execute(0, mode, operand, cycles);
  }
  return cycles;
}

uint16_t InstructionTestBase::ExpectImmediate() {
  reg_.pc = 0x00;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Peripheral::Status::OK)));
  return 0x12;
}

void InstructionTestBase::ExpectPostImmediate() { EXPECT_EQ(reg_.pc, 0x01); }

uint16_t InstructionTestBase::ExpectZeroPage() {
  reg_.pc = 0x00;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Peripheral::Status::OK)));
  return 0x12;
}

void InstructionTestBase::ExpectPostZeroPage() { EXPECT_EQ(reg_.pc, 0x01); }

uint16_t InstructionTestBase::ExpectIndexedZeroPage(uint8_t& index) {
  index = 7;
  reg_.pc = 0x00;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Peripheral::Status::OK)));
  return 0x12 + 7;
}

void InstructionTestBase::ExpectPostIndexedZeroPage() {
  EXPECT_EQ(reg_.pc, 0x01);
}

uint16_t InstructionTestBase::ExpectAbsolute() {
  reg_.pc = 0x00;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x01, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Peripheral::Status::OK)));
  return 0x3412;
}

void InstructionTestBase::ExpectPostAbsolute() { EXPECT_EQ(reg_.pc, 0x02); }

uint16_t InstructionTestBase::ExpectIndexedAbsolute(uint8_t& index) {
  index = 7;
  reg_.pc = 0x00;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x01, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Peripheral::Status::OK)));
  return 0x3412 + 7;
}

void InstructionTestBase::ExpectPostIndexedAbsolute() {
  EXPECT_EQ(reg_.pc, 0x02);
}

uint16_t InstructionTestBase::ExpectIndexedIndirect(uint8_t& index) {
  index = 7;
  reg_.pc = 0x00;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x19, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x1A, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x56), Return(Peripheral::Status::OK)));
  return 0x5634;
}

void InstructionTestBase::ExpectPostIndexedIndirect() {
  EXPECT_EQ(reg_.pc, 0x01);
}

uint16_t InstructionTestBase::ExpectIndirectIndexed(uint8_t& index) {
  index = 7;
  reg_.pc = 0x00;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x12, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x13, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x56), Return(Peripheral::Status::OK)));
  return 0x5634 + 7;
}

void InstructionTestBase::ExpectPostIndirectIndexed() {
  EXPECT_EQ(reg_.pc, 0x01);
}
