#include "src/engine/instruction/jump.h"

#include "src/engine/instruction/instruction_test_base.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

namespace {

class JumpTest : public InstructionTestBase {
 protected:
  JMP jmp_{bus_, reg_};
  JSR jsr_{bus_, reg_};
  RTS rts_{bus_, reg_};
  RTI rti_{bus_, reg_};
};

TEST_F(JumpTest, JSR_RTS) {
  reg_.pc = 0x1124;
  reg_.sp = 0x10;

  EXPECT_CALL(bus_, Write(0x110, 0x11));
  EXPECT_CALL(bus_, Write(0x10F, 0x23));

  jsr_.Execute(0xBBAA);
  EXPECT_EQ(reg_.pc, 0xBBAA);
  EXPECT_EQ(reg_.sp, 0x0E);

  EXPECT_CALL(bus_, Read(0x10F, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x23), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x110, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x11), Return(Peripheral::Status::OK)));

  rts_.Execute(0);
  EXPECT_EQ(reg_.pc, 0x1124);
  EXPECT_EQ(reg_.sp, 0x10);
}

TEST_F(JumpTest, Timing) {
  EXPECT_EQ(3, TimeInstruction(&jmp_, Instruction::Mode::kImmediateAddress,
                               Instruction::Operation::kRead));
  EXPECT_EQ(5, TimeInstruction(&jmp_, Instruction::Mode::kIndirect,
                               Instruction::Operation::kRead));
  EXPECT_EQ(6, TimeInstruction(&jsr_, Instruction::Mode::kImmediateAddress,
                               Instruction::Operation::kRead));
  EXPECT_EQ(6, TimeInstruction(&rts_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(6, TimeInstruction(&rti_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
}

}  // namespace
