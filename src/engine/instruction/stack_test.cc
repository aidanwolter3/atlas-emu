#include "src/engine/instruction/stack.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

class StackTest : public InstructionTestBase {};

TEST_F(StackTest, PHA) {
  reg_.sp = 0xFF;
  reg_.acc = 0xAB;
  PHA pha(bus_, reg_);
  EXPECT_CALL(bus_, Write(0x01FF, 0xAB))
      .WillOnce(Return(Peripheral::Status::OK));
  pha.Execute(0x48);
  EXPECT_EQ(reg_.sp, 0xFE);
}

TEST_F(StackTest, PLA) {
  reg_.sp = 0xFE;
  reg_.acc = 0;
  PLA pla(bus_, reg_);
  EXPECT_CALL(bus_, Read(0x01FF, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAB), Return(Peripheral::Status::OK)));
  pla.Execute(0x68);
  EXPECT_EQ(reg_.sp, 0xFF);
  EXPECT_EQ(reg_.acc, 0xAB);
}

TEST_F(StackTest, PHP) {
  reg_.sp = 0xFF;
  reg_.status = 0xAB;
  PHP php(bus_, reg_);
  EXPECT_CALL(bus_, Write(0x01FF, 0xAB))
      .WillOnce(Return(Peripheral::Status::OK));
  php.Execute(0x08);
  EXPECT_EQ(reg_.sp, 0xFE);
}

TEST_F(StackTest, PLP) {
  reg_.sp = 0xFE;
  reg_.status = 0;
  PLP plp(bus_, reg_);
  EXPECT_CALL(bus_, Read(0x01FF, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAB), Return(Peripheral::Status::OK)));
  plp.Execute(0x28);
  EXPECT_EQ(reg_.sp, 0xFF);
  EXPECT_EQ(reg_.status, 0xAB);
}

}  // namespace
