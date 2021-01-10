#include "src/engine/instruction/misc.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class MiscTest : public Instruction2TestBase {};

TEST_F(MiscTest, NOP) {
  reg_.pc = 0;
  reg_.status = 0;
  NOP nop(bus_, reg_);
  int cycles = ExecuteUntilComplete(&nop);
  EXPECT_EQ(cycles, 2);
  EXPECT_EQ(reg_.pc, 0);
  EXPECT_EQ(reg_.status, 0);
}

}  // namespace
