#include "src/engine/instruction/logic.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class LogicTest : public Instruction2TestBase {};

TEST_F(LogicTest, BIT_ZeroPage) {
  reg_.acc = 0x80;
  reg_.pc = 0;
  int cycles = 0;
  BIT bit(bus_, reg_);

  cycles = ExecuteUntilComplete(&bit, Instruction2::Mode::kZeroPage, 0xFF);
  EXPECT_EQ(cycles, 3);
  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_TRUE(reg_.status.test(Status::kSign));
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));

  cycles = ExecuteUntilComplete(&bit, Instruction2::Mode::kZeroPage, 0x7F);
  EXPECT_EQ(cycles, 3);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));

  cycles = ExecuteUntilComplete(&bit, Instruction2::Mode::kZeroPage, 0x3F);
  EXPECT_EQ(cycles, 3);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
}

}  // namespace
