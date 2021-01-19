#include "src/engine/instruction/branch.h"

#include "src/engine/instruction/addressing.h"
#include "src/engine/instruction/instruction.h"
#include "src/engine/instruction/instruction_test_base.h"

namespace {

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

class BranchTest : public InstructionTestBase {};

TEST_F(BranchTest, BEQ) {
  reg_.pc = 0;
  BEQ beq(bus_, reg_);

  reg_.status.set(Status::kZero, false);
  beq.Execute(4);
  EXPECT_EQ(reg_.pc, 0);

  reg_.status.set(Status::kZero, true);
  beq.Execute(4);
  EXPECT_EQ(reg_.pc, 4);
}

TEST_F(BranchTest, BEQ_Timing) {
  Addressing addressing(bus_, reg_);
  BEQ beq(bus_, reg_);
  int total_ticks;
  bool finished;

  Instruction::Config config = {
      .opcode = 0,
      .mode = Instruction::Mode::kImmediate,
      .operation = Instruction::Operation::kRead,
      .instruction = &beq,
  };

  // Ensure no branch takes 2 cycles.
  total_ticks = 0;
  reg_.pc = 0;
  reg_.status.set(Status::kZero, false);
  EXPECT_CALL(bus_, Read(0, _))
      .WillOnce(DoAll(SetArgPointee<1>(4), Return(Peripheral::Status::OK)));
  finished = false;
  while (!finished) {
    total_ticks++;
    finished = addressing.Execute(config, total_ticks);
  }
  EXPECT_EQ(total_ticks, 2);

  // Ensure a branch takes 3 cycles.
  total_ticks = 0;
  reg_.pc = 0;
  reg_.status.set(Status::kZero, true);
  EXPECT_CALL(bus_, Read(0, _))
      .WillOnce(DoAll(SetArgPointee<1>(4), Return(Peripheral::Status::OK)));
  finished = false;
  while (!finished) {
    total_ticks++;
    finished = addressing.Execute(config, total_ticks);
  }
  EXPECT_EQ(total_ticks, 3);
}

}  // namespace
