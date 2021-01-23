#include "src/engine/instruction/branch.h"

#include "src/engine/instruction/instruction.h"
#include "src/engine/instruction/instruction_test_base.h"

namespace {

class BranchTest : public InstructionTestBase {
 protected:
  BEQ beq_{bus_, reg_};
};

TEST_F(BranchTest, BEQ) {
  reg_.pc = 0;
  reg_.status.set(Status::kZero, false);
  beq_.Execute(4);
  EXPECT_EQ(reg_.pc, 0);

  reg_.pc = 0;
  reg_.status.set(Status::kZero, true);
  beq_.Execute(4);
  EXPECT_EQ(reg_.pc, 4);
}

TEST_F(BranchTest, Timing) {
  reg_.status.set(Status::kZero, false);
  EXPECT_EQ(2, TimeInstruction(&beq_, Instruction::Mode::kImmediate,
                               Instruction::Operation::kRead));

  reg_.status.set(Status::kZero, true);
  EXPECT_EQ(3, TimeInstruction(&beq_, Instruction::Mode::kImmediate,
                               Instruction::Operation::kRead));
}

}  // namespace
