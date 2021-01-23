#include "src/engine/instruction/compare.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class CompareTest : public InstructionTestBase {
 protected:
  void ExpectZeroCarrySign(bool zero, bool carry, bool sign) {
    EXPECT_EQ(zero, reg_.status.test(Status::kZero));
    EXPECT_EQ(carry, reg_.status.test(Status::kCarry));
    EXPECT_EQ(sign, reg_.status.test(Status::kSign));
  }

  CMP cmp_{bus_, reg_};
};

TEST_F(CompareTest, CMP) {
  reg_.acc = 10;
  cmp_.Execute(10);
  ExpectZeroCarrySign(true, true, false);

  cmp_.Execute(20);
  ExpectZeroCarrySign(false, false, true);

  cmp_.Execute(0);
  ExpectZeroCarrySign(false, true, false);

  reg_.acc = 0xF0;
  cmp_.Execute(10);
  ExpectZeroCarrySign(false, true, true);
}

TEST_F(CompareTest, Timing) {
  EXPECT_EQ(2, TimeInstruction(&cmp_, Instruction::Mode::kImmediate,
                               Instruction::Operation::kRead));
  EXPECT_EQ(3, TimeInstruction(&cmp_, Instruction::Mode::kZeroPage,
                               Instruction::Operation::kRead));
  EXPECT_EQ(4, TimeInstruction(&cmp_, Instruction::Mode::kZeroPageX,
                               Instruction::Operation::kRead));
  EXPECT_EQ(4, TimeInstruction(&cmp_, Instruction::Mode::kAbsolute,
                               Instruction::Operation::kRead));
  EXPECT_EQ(5, TimeInstruction(&cmp_, Instruction::Mode::kAbsoluteX,
                               Instruction::Operation::kRead));
  EXPECT_EQ(5, TimeInstruction(&cmp_, Instruction::Mode::kAbsoluteY,
                               Instruction::Operation::kRead));
  EXPECT_EQ(6, TimeInstruction(&cmp_, Instruction::Mode::kIndirectX,
                               Instruction::Operation::kRead));
  EXPECT_EQ(6, TimeInstruction(&cmp_, Instruction::Mode::kIndirectY,
                               Instruction::Operation::kRead));
}

}  // namespace
