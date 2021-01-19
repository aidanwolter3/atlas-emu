#include "src/engine/instruction/store.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class StoreTest : public InstructionTestBase {};

TEST_F(StoreTest, STA) {
  reg_.acc = 0xAA;
  STA sta(bus_, reg_);
  uint8_t result = sta.Execute(0).data;
  EXPECT_EQ(result, 0xAA);
}

TEST_F(StoreTest, STX) {
  reg_.x = 0xBB;
  STX stx(bus_, reg_);
  uint8_t result = stx.Execute(0).data;
  EXPECT_EQ(result, 0xBB);
}

TEST_F(StoreTest, STY) {
  reg_.y = 0xCC;
  STY sty(bus_, reg_);
  uint8_t result = sty.Execute(0).data;
  EXPECT_EQ(result, 0xCC);
}

}  // namespace
