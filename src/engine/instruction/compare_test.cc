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
};

TEST_F(CompareTest, CMP) {
  reg_.acc = 10;
  CMP cmp(bus_, reg_);

  cmp.Execute(10);
  ExpectZeroCarrySign(true, true, false);

  cmp.Execute(20);
  ExpectZeroCarrySign(false, false, true);

  cmp.Execute(0);
  ExpectZeroCarrySign(false, true, false);

  reg_.acc = 0xF0;
  cmp.Execute(10);
  ExpectZeroCarrySign(false, true, true);
}

}  // namespace
