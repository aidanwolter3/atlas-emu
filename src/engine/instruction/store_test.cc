#include "src/engine/instruction/store.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class StoreTest : public InstructionTestBase {
 protected:
  STA sta_{bus_, reg_};
  STX stx_{bus_, reg_};
  STY sty_{bus_, reg_};
};

TEST_F(StoreTest, STA) {
  reg_.acc = 0xAA;
  uint8_t result = sta_.Execute(0).data;
  EXPECT_EQ(result, 0xAA);
}

TEST_F(StoreTest, STX) {
  reg_.x = 0xBB;
  uint8_t result = stx_.Execute(0).data;
  EXPECT_EQ(result, 0xBB);
}

TEST_F(StoreTest, STY) {
  reg_.y = 0xCC;
  uint8_t result = sty_.Execute(0).data;
  EXPECT_EQ(result, 0xCC);
}

TEST_F(StoreTest, Timing) {
  EXPECT_EQ(3, TimeInstruction(&sta_, Instruction::Mode::kZeroPage,
                               Instruction::Operation::kWrite));
  EXPECT_EQ(4, TimeInstruction(&sta_, Instruction::Mode::kZeroPageX,
                               Instruction::Operation::kWrite));
  EXPECT_EQ(4, TimeInstruction(&sta_, Instruction::Mode::kAbsolute,
                               Instruction::Operation::kWrite));
  EXPECT_EQ(5, TimeInstruction(&sta_, Instruction::Mode::kAbsoluteX,
                               Instruction::Operation::kWrite));
  EXPECT_EQ(5, TimeInstruction(&sta_, Instruction::Mode::kAbsoluteY,
                               Instruction::Operation::kWrite));
  EXPECT_EQ(6, TimeInstruction(&sta_, Instruction::Mode::kIndirectX,
                               Instruction::Operation::kWrite));
  EXPECT_EQ(6, TimeInstruction(&sta_, Instruction::Mode::kIndirectY,
                               Instruction::Operation::kWrite));
}

}  // namespace
