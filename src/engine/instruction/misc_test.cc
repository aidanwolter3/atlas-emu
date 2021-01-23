#include "src/engine/instruction/misc.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class MiscTest : public InstructionTestBase {
 protected:
  NOP nop_{bus_, reg_};
};

TEST_F(MiscTest, NOP) {
  reg_.pc = 0;
  reg_.status = 0;

  nop_.Execute(0);
  EXPECT_EQ(reg_.pc, 0);
  EXPECT_EQ(reg_.status, 0);
}

TEST_F(MiscTest, Timing) {
  EXPECT_EQ(2, TimeInstruction(&nop_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
}

}  // namespace
