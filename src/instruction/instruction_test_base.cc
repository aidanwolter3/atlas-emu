#include "src/instruction/instruction_test_base.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

uint16_t InstructionTestBase::ExpectImmediate() {
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  return 0x12;
}

void InstructionTestBase::ExpectPostImmediate() { EXPECT_EQ(reg_.pc, 0x01); }

uint16_t InstructionTestBase::ExpectZeroPage() {
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  return 0x12;
}

void InstructionTestBase::ExpectPostZeroPage() { EXPECT_EQ(reg_.pc, 0x01); }

uint16_t InstructionTestBase::ExpectIndexedZeroPage(uint8_t& index) {
  index = 7;
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  return 0x12 + 7;
}

void InstructionTestBase::ExpectPostIndexedZeroPage() {
  EXPECT_EQ(reg_.pc, 0x01);
}

uint16_t InstructionTestBase::ExpectAbsolute() {
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x01, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  return 0x3412;
}

void InstructionTestBase::ExpectPostAbsolute() { EXPECT_EQ(reg_.pc, 0x02); }

uint16_t InstructionTestBase::ExpectIndexedAbsolute(uint8_t& index) {
  index = 7;
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x01, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  return 0x3412 + 7;
}

void InstructionTestBase::ExpectPostIndexedAbsolute() {
  EXPECT_EQ(reg_.pc, 0x02);
}

uint16_t InstructionTestBase::ExpectIndexedIndirect(uint8_t& index) {
  index = 7;
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x19, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  return 0x34;
}

void InstructionTestBase::ExpectPostIndexedIndirect() {
  EXPECT_EQ(reg_.pc, 0x01);
}

uint16_t InstructionTestBase::ExpectIndirectIndexed(uint8_t& index) {
  index = 7;
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x12, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  return 0x34 + 7;
}

void InstructionTestBase::ExpectPostIndirectIndexed() {
  EXPECT_EQ(reg_.pc, 0x01);
}