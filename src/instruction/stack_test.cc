#include "src/instruction/stack.h"

#include "src/instruction/instruction_test_base.h"

namespace {

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

class StackTest : public InstructionTestBase {};

TEST_F(StackTest, TXS) {
  reg_.sp = 0;
  reg_.x = 0xAB;
  TXS txs(mem_, reg_);
  txs.Execute(0x9A);
  EXPECT_EQ(reg_.sp, 0xAB);
}

TEST_F(StackTest, TSX) {
  reg_.sp = 0xAB;
  reg_.x = 0;
  TSX tsx(mem_, reg_);
  tsx.Execute(0xBA);
  EXPECT_EQ(reg_.x, 0xAB);
}

TEST_F(StackTest, PHA) {
  reg_.sp = 0xFF;
  reg_.acc = 0xAB;
  PHA pha(mem_, reg_);
  EXPECT_CALL(mem_, Write(0x01FF, 0xAB))
      .WillOnce(Return(Peripheral::Status::OK));
  pha.Execute(0x48);
  EXPECT_EQ(reg_.sp, 0xFE);
}

TEST_F(StackTest, PLA) {
  reg_.sp = 0xFE;
  reg_.acc = 0;
  PLA pla(mem_, reg_);
  EXPECT_CALL(mem_, Read(0x01FF, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAB), Return(Peripheral::Status::OK)));
  pla.Execute(0x68);
  EXPECT_EQ(reg_.sp, 0xFF);
  EXPECT_EQ(reg_.acc, 0xAB);
}

TEST_F(StackTest, PHP) {
  reg_.sp = 0xFF;
  reg_.status = 0xAB;
  PHP php(mem_, reg_);
  EXPECT_CALL(mem_, Write(0x01FF, 0xAB))
      .WillOnce(Return(Peripheral::Status::OK));
  php.Execute(0x08);
  EXPECT_EQ(reg_.sp, 0xFE);
}

TEST_F(StackTest, PLP) {
  reg_.sp = 0xFE;
  reg_.status = 0;
  PLP plp(mem_, reg_);
  EXPECT_CALL(mem_, Read(0x01FF, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAB), Return(Peripheral::Status::OK)));
  plp.Execute(0x28);
  EXPECT_EQ(reg_.sp, 0xFF);
  EXPECT_EQ(reg_.status, 0xAB);
}

}  // namespace
