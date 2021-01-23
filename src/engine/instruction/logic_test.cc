#include "src/engine/instruction/logic.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class LogicTest : public InstructionTestBase {
 protected:
  AND and_{bus_, reg_};
  BIT bit_{bus_, reg_};
};

TEST_F(LogicTest, BIT_ZeroPage) {
  reg_.acc = 0x80;
  reg_.pc = 0;

  bit_.Execute(0xFF);
  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_TRUE(reg_.status.test(Status::kSign));
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));

  bit_.Execute(0x7F);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));

  bit_.Execute(0x3F);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
}

TEST_F(LogicTest, Timing) {
  EXPECT_EQ(2, TimeInstruction(&and_, Instruction::Mode::kImmediate,
                               Instruction::Operation::kRead));
  EXPECT_EQ(3, TimeInstruction(&and_, Instruction::Mode::kZeroPage,
                               Instruction::Operation::kRead));
  EXPECT_EQ(4, TimeInstruction(&and_, Instruction::Mode::kZeroPageX,
                               Instruction::Operation::kRead));
  EXPECT_EQ(4, TimeInstruction(&and_, Instruction::Mode::kAbsolute,
                               Instruction::Operation::kRead));
  EXPECT_EQ(5, TimeInstruction(&and_, Instruction::Mode::kAbsoluteX,
                               Instruction::Operation::kRead));
  EXPECT_EQ(5, TimeInstruction(&and_, Instruction::Mode::kAbsoluteY,
                               Instruction::Operation::kRead));
  EXPECT_EQ(6, TimeInstruction(&and_, Instruction::Mode::kIndirectX,
                               Instruction::Operation::kRead));
  EXPECT_EQ(6, TimeInstruction(&and_, Instruction::Mode::kIndirectY,
                               Instruction::Operation::kRead));

  EXPECT_EQ(3, TimeInstruction(&bit_, Instruction::Mode::kZeroPage,
                               Instruction::Operation::kRead));
  EXPECT_EQ(4, TimeInstruction(&bit_, Instruction::Mode::kAbsolute,
                               Instruction::Operation::kRead));
}

}  // namespace
