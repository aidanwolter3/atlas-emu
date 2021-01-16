#include "src/engine/instruction/load.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class LoadTest : public Instruction2TestBase {};

TEST_F(LoadTest, LDA) {
  LDA lda(bus_, reg_);
  lda.Execute(0, 0xAA);
  EXPECT_EQ(reg_.acc, 0xAA);
  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_TRUE(reg_.status.test(Status::kSign));

  lda.Execute(0, 0x00);
  EXPECT_EQ(reg_.acc, 0x00);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
}

TEST_F(LoadTest, LDX) {
  LDX ldx(bus_, reg_);
  ldx.Execute(0, 0xBB);
  EXPECT_EQ(reg_.x, 0xBB);

  ldx.Execute(0, 0x00);
  EXPECT_EQ(reg_.x, 0x00);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
}

TEST_F(LoadTest, LDY) {
  LDY ldy(bus_, reg_);
  ldy.Execute(0, 0xCC);
  EXPECT_EQ(reg_.y, 0xCC);

  ldy.Execute(0, 0x00);
  EXPECT_EQ(reg_.y, 0x00);
  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
}

}  // namespace
