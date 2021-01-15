#include "src/engine/instruction/compare.h"

#include "src/engine/instruction/instruction_test_base.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

namespace {

class CompareTest : public Instruction2TestBase {
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

  reg_.pc = 0;
  cmp.Execute(0, 10);
  ExpectZeroCarrySign(true, true, false);

  reg_.pc = 0;
  cmp.Execute(0, 20);
  ExpectZeroCarrySign(false, false, true);

  reg_.pc = 0;
  cmp.Execute(0, 0);
  ExpectZeroCarrySign(false, true, false);
}

}  // namespace
