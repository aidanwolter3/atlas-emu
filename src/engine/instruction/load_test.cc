#include "src/engine/instruction/load.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

class LoadTest : public InstructionTestBase {
 protected:
  uint8_t ExpectRead(uint16_t address);
  void TestImmediate(Instruction& instruction, uint8_t& dest, uint8_t opcode);
  void TestZeroPage(Instruction& instruction, uint8_t& dest, uint8_t opcode);
  void TestIndexedZeroPage(Instruction& instruction, uint8_t& dest,
                           uint8_t& index, uint8_t opcode);
  void TestAbsolute(Instruction& instruction, uint8_t& dest, uint8_t opcode);
  void TestIndexedAbsolute(Instruction& instruction, uint8_t& dest,
                           uint8_t& index, uint8_t opcode);
  void TestIndexedIndirect(Instruction& instruction, uint8_t& dest,
                           uint8_t& index, uint8_t opcode);
  void TestIndirectIndexed(Instruction& instruction, uint8_t& dest,
                           uint8_t& index, uint8_t opcode);
  void TestStatusZero(Instruction& instruction, uint8_t immediate_opcode);
  void TestStatusNegative(Instruction& instruction, uint8_t immediate_opcode);
};

uint8_t LoadTest::ExpectRead(uint16_t address) {
  EXPECT_CALL(bus_, Read(address, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAB), Return(Peripheral::Status::OK)));
  return 0xAB;
}

void LoadTest::TestImmediate(Instruction& instruction, uint8_t& dest,
                             uint8_t opcode) {
  auto data = ExpectImmediate();
  instruction.Execute(opcode);
  EXPECT_EQ(dest, data);
  ExpectPostImmediate();
}

void LoadTest::TestZeroPage(Instruction& instruction, uint8_t& dest,
                            uint8_t opcode) {
  auto data = ExpectRead(ExpectZeroPage());
  instruction.Execute(opcode);
  EXPECT_EQ(dest, data);
  ExpectPostZeroPage();
}

void LoadTest::TestIndexedZeroPage(Instruction& instruction, uint8_t& dest,
                                   uint8_t& index, uint8_t opcode) {
  auto data = ExpectRead(ExpectIndexedZeroPage(index));
  instruction.Execute(opcode);
  EXPECT_EQ(dest, data);
  ExpectPostIndexedZeroPage();
}

void LoadTest::TestAbsolute(Instruction& instruction, uint8_t& dest,
                            uint8_t opcode) {
  auto data = ExpectRead(ExpectAbsolute());
  instruction.Execute(opcode);
  EXPECT_EQ(dest, data);
  ExpectPostAbsolute();
}

void LoadTest::TestIndexedAbsolute(Instruction& instruction, uint8_t& dest,
                                   uint8_t& index, uint8_t opcode) {
  auto data = ExpectRead(ExpectIndexedAbsolute(index));
  instruction.Execute(opcode);
  EXPECT_EQ(dest, data);
  ExpectPostIndexedAbsolute();
}

void LoadTest::TestIndexedIndirect(Instruction& instruction, uint8_t& dest,
                                   uint8_t& index, uint8_t opcode) {
  auto data = ExpectRead(ExpectIndexedIndirect(index));
  instruction.Execute(opcode);
  EXPECT_EQ(dest, data);
  ExpectPostIndexedIndirect();
}

void LoadTest::TestIndirectIndexed(Instruction& instruction, uint8_t& dest,
                                   uint8_t& index, uint8_t opcode) {
  auto data = ExpectRead(ExpectIndirectIndexed(index));
  instruction.Execute(opcode);
  EXPECT_EQ(dest, data);
  ExpectPostIndirectIndexed();
}

void LoadTest::TestStatusZero(Instruction& instruction,
                              uint8_t immediate_opcode) {
  reg_.pc = 0x00;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x00), Return(Peripheral::Status::OK)));
  instruction.Execute(immediate_opcode);

  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
}

void LoadTest::TestStatusNegative(Instruction& instruction,
                                  uint8_t immediate_opcode) {
  reg_.pc = 0x00;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xFF), Return(Peripheral::Status::OK)));
  instruction.Execute(immediate_opcode);

  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_TRUE(reg_.status.test(Status::kSign));
}

TEST_F(LoadTest, LDA) {
  LDA lda(bus_, reg_);
  TestImmediate(lda, reg_.acc, 0xA9);
  TestZeroPage(lda, reg_.acc, 0xA5);
  TestIndexedZeroPage(lda, reg_.acc, reg_.x, 0xB5);
  TestAbsolute(lda, reg_.acc, 0xAD);
  TestIndexedAbsolute(lda, reg_.acc, reg_.x, 0xBD);
  TestIndexedAbsolute(lda, reg_.acc, reg_.y, 0xB9);
  TestIndexedIndirect(lda, reg_.acc, reg_.x, 0xA1);
  TestIndirectIndexed(lda, reg_.acc, reg_.y, 0xB1);
  TestStatusZero(lda, 0xA9);
  TestStatusNegative(lda, 0xA9);
}

TEST_F(LoadTest, LDX) {
  LDX ldx(bus_, reg_);
  TestImmediate(ldx, reg_.x, 0xA2);
  TestZeroPage(ldx, reg_.x, 0xA6);
  TestIndexedZeroPage(ldx, reg_.x, reg_.y, 0xB6);
  TestAbsolute(ldx, reg_.x, 0xAE);
  TestIndexedAbsolute(ldx, reg_.x, reg_.y, 0xBE);
  TestStatusZero(ldx, 0xA2);
  TestStatusNegative(ldx, 0xA2);
}

TEST_F(LoadTest, LDY) {
  LDY ldy(bus_, reg_);
  TestImmediate(ldy, reg_.y, 0xA0);
  TestZeroPage(ldy, reg_.y, 0xA4);
  TestIndexedZeroPage(ldy, reg_.y, reg_.x, 0xB4);
  TestAbsolute(ldy, reg_.y, 0xAC);
  TestIndexedAbsolute(ldy, reg_.y, reg_.x, 0xBC);
  TestStatusZero(ldy, 0xA0);
  TestStatusNegative(ldy, 0xA0);
}

}  // namespace
