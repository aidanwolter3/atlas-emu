#include "src/instruction/misc.h"

#include "src/instruction/instruction_test_base.h"

namespace {

class MiscTest : public InstructionTestBase {};

TEST_F(MiscTest, NOP) {
  reg_.pc = 0;
  reg_.status = 0;
  NOP nop(bus_, reg_);
  nop.Execute(0x78);
  EXPECT_EQ(reg_.pc, 0);
  EXPECT_EQ(reg_.status, 0);
}

}  // namespace
