#include "src/engine/instruction/logic.h"

#include "src/engine/instruction/instruction_test_base.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

namespace {

class LogicTest : public InstructionTestBase {};

TEST_F(LogicTest, BIT_ZeroPage) {
  reg_.acc = 0x80;
  reg_.pc = 0;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x10), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x10, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xFF), Return(Peripheral::Status::OK)));

  BIT bit(bus_, reg_);
  bit.Execute(0x24);

  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_TRUE(reg_.status.test(Status::kSign));
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));

  reg_.pc = 0;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x10), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x10, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x7F), Return(Peripheral::Status::OK)));

  bit.Execute(0x24);

  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));

  reg_.pc = 0;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x10), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x10, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x3F), Return(Peripheral::Status::OK)));

  bit.Execute(0x24);

  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
}

}  // namespace
