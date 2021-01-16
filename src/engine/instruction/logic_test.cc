#include "src/engine/instruction/logic.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class LogicTest : public InstructionTestBase {};

TEST_F(LogicTest, BIT_ZeroPage) {
  reg_.acc = 0x80;
  reg_.pc = 0;
  BIT bit(bus_, reg_);

  bit.Execute(0xFF);
  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_TRUE(reg_.status.test(Status::kSign));
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));

  bit.Execute(0x7F);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));

  bit.Execute(0x3F);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
}

}  // namespace
