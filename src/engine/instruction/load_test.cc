#include "src/engine/instruction/load.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class LoadTest : public InstructionTestBase {
 protected:
  LDA lda_{bus_, reg_};
  LDX ldx_{bus_, reg_};
  LDY ldy_{bus_, reg_};
};

TEST_F(LoadTest, LDA) {
  lda_.Execute(0xAA);
  EXPECT_EQ(reg_.acc, 0xAA);
  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_TRUE(reg_.status.test(Status::kSign));

  lda_.Execute(0x00);
  EXPECT_EQ(reg_.acc, 0x00);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
}

TEST_F(LoadTest, LDX) {
  ldx_.Execute(0xBB);
  EXPECT_EQ(reg_.x, 0xBB);

  ldx_.Execute(0x00);
  EXPECT_EQ(reg_.x, 0x00);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
}

TEST_F(LoadTest, LDY) {
  ldy_.Execute(0xCC);
  EXPECT_EQ(reg_.y, 0xCC);

  ldy_.Execute(0x00);
  EXPECT_EQ(reg_.y, 0x00);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
}

TEST_F(LoadTest, Timing) {
  EXPECT_EQ(2, TimeInstruction(&lda_, Instruction::Mode::kImmediate,
                               Instruction::Operation::kRead));
  EXPECT_EQ(3, TimeInstruction(&lda_, Instruction::Mode::kZeroPage,
                               Instruction::Operation::kRead));
  EXPECT_EQ(4, TimeInstruction(&lda_, Instruction::Mode::kZeroPageX,
                               Instruction::Operation::kRead));
  EXPECT_EQ(4, TimeInstruction(&lda_, Instruction::Mode::kAbsolute,
                               Instruction::Operation::kRead));
  EXPECT_EQ(5, TimeInstruction(&lda_, Instruction::Mode::kAbsoluteX,
                               Instruction::Operation::kRead));
  EXPECT_EQ(5, TimeInstruction(&lda_, Instruction::Mode::kAbsoluteY,
                               Instruction::Operation::kRead));
  EXPECT_EQ(6, TimeInstruction(&lda_, Instruction::Mode::kIndirectX,
                               Instruction::Operation::kRead));
  EXPECT_EQ(6, TimeInstruction(&lda_, Instruction::Mode::kIndirectY,
                               Instruction::Operation::kRead));
}

}  // namespace
