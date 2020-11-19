#include "src/instruction/status.h"

#include "src/instruction/instruction_test_base.h"

namespace {

class StatusTest : public InstructionTestBase {};

TEST_F(StatusTest, CLC) {
  reg_.status.set(Status::kCarry);
  CLC clc(bus_, reg_);
  clc.Execute(0x18);
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(StatusTest, SEC) {
  reg_.status.reset(Status::kCarry);
  SEC sec(bus_, reg_);
  sec.Execute(0x38);
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

TEST_F(StatusTest, CLI) {
  reg_.status.set(Status::kIntDisable);
  CLI cli(bus_, reg_);
  cli.Execute(0x58);
  EXPECT_FALSE(reg_.status.test(Status::kIntDisable));
}

TEST_F(StatusTest, SEI) {
  reg_.status.reset(Status::kIntDisable);
  SEI sei(bus_, reg_);
  sei.Execute(0x78);
  EXPECT_TRUE(reg_.status.test(Status::kIntDisable));
}

TEST_F(StatusTest, CLV) {
  reg_.status.set(Status::kOverflow);
  CLV clv(bus_, reg_);
  clv.Execute(0xB8);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
}

TEST_F(StatusTest, CLD) {
  reg_.status.set(Status::kBCDMode);
  CLD cld(bus_, reg_);
  cld.Execute(0xD8);
  EXPECT_FALSE(reg_.status.test(Status::kBCDMode));
}

TEST_F(StatusTest, SED) {
  reg_.status.reset(Status::kBCDMode);
  SED sed(bus_, reg_);
  sed.Execute(0xF8);
  EXPECT_TRUE(reg_.status.test(Status::kBCDMode));
}

}  // namespace
