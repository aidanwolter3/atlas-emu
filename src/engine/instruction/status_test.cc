#include "src/engine/instruction/status.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class StatusTest : public Instruction2TestBase {};

TEST_F(StatusTest, CLC) {
  reg_.status.set(Status::kCarry);
  CLC clc(bus_, reg_);
  int cycles = ExecuteUntilComplete(&clc);
  EXPECT_EQ(cycles, 2);
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(StatusTest, SEC) {
  reg_.status.reset(Status::kCarry);
  SEC sec(bus_, reg_);
  int cycles = ExecuteUntilComplete(&sec);
  EXPECT_EQ(cycles, 2);
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

TEST_F(StatusTest, CLI) {
  reg_.status.set(Status::kIntDisable);
  CLI cli(bus_, reg_);
  int cycles = ExecuteUntilComplete(&cli);
  EXPECT_EQ(cycles, 2);
  EXPECT_FALSE(reg_.status.test(Status::kIntDisable));
}

TEST_F(StatusTest, SEI) {
  reg_.status.reset(Status::kIntDisable);
  SEI sei(bus_, reg_);
  int cycles = ExecuteUntilComplete(&sei);
  EXPECT_EQ(cycles, 2);
  EXPECT_TRUE(reg_.status.test(Status::kIntDisable));
}

TEST_F(StatusTest, CLV) {
  reg_.status.set(Status::kOverflow);
  CLV clv(bus_, reg_);
  int cycles = ExecuteUntilComplete(&clv);
  EXPECT_EQ(cycles, 2);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
}

TEST_F(StatusTest, CLD) {
  reg_.status.set(Status::kBCDMode);
  CLD cld(bus_, reg_);
  int cycles = ExecuteUntilComplete(&cld);
  EXPECT_EQ(cycles, 2);
  EXPECT_FALSE(reg_.status.test(Status::kBCDMode));
}

TEST_F(StatusTest, SED) {
  reg_.status.reset(Status::kBCDMode);
  SED sed(bus_, reg_);
  int cycles = ExecuteUntilComplete(&sed);
  EXPECT_EQ(cycles, 2);
  EXPECT_TRUE(reg_.status.test(Status::kBCDMode));
}

}  // namespace
