#include "src/engine/instruction/status.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class StatusTest : public InstructionTestBase {
 protected:
  CLC clc_{bus_, reg_};
  SEC sec_{bus_, reg_};
  CLI cli_{bus_, reg_};
  SEI sei_{bus_, reg_};
  CLV clv_{bus_, reg_};
  CLD cld_{bus_, reg_};
  SED sed_{bus_, reg_};
};

TEST_F(StatusTest, CLC) {
  reg_.status.set(Status::kCarry);
  clc_.Execute(0);
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(StatusTest, SEC) {
  reg_.status.reset(Status::kCarry);
  sec_.Execute(0);
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

TEST_F(StatusTest, CLI) {
  reg_.status.set(Status::kIntDisable);
  cli_.Execute(0);
  EXPECT_FALSE(reg_.status.test(Status::kIntDisable));
}

TEST_F(StatusTest, SEI) {
  reg_.status.reset(Status::kIntDisable);
  sei_.Execute(0);
  EXPECT_TRUE(reg_.status.test(Status::kIntDisable));
}

TEST_F(StatusTest, CLV) {
  reg_.status.set(Status::kOverflow);
  clv_.Execute(0);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
}

TEST_F(StatusTest, CLD) {
  reg_.status.set(Status::kBCDMode);
  cld_.Execute(0);
  EXPECT_FALSE(reg_.status.test(Status::kBCDMode));
}

TEST_F(StatusTest, SED) {
  reg_.status.reset(Status::kBCDMode);
  sed_.Execute(0);
  EXPECT_TRUE(reg_.status.test(Status::kBCDMode));
}

TEST_F(StatusTest, Timing) {
  EXPECT_EQ(2, TimeInstruction(&clc_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(2, TimeInstruction(&sec_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(2, TimeInstruction(&cli_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(2, TimeInstruction(&sei_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(2, TimeInstruction(&clv_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(2, TimeInstruction(&cld_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(2, TimeInstruction(&sed_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
}

}  // namespace
