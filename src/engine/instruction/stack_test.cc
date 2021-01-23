#include "src/engine/instruction/stack.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

class StackTest : public InstructionTestBase {
 protected:
  PHA pha_{bus_, reg_};
  PLA pla_{bus_, reg_};
  PHP php_{bus_, reg_};
  PLP plp_{bus_, reg_};
};

TEST_F(StackTest, PHA) {
  reg_.sp = 0xFF;
  reg_.acc = 0xAB;
  EXPECT_CALL(bus_, Write(0x01FF, 0xAB))
      .WillOnce(Return(Peripheral::Status::OK));
  pha_.Execute(0);
  EXPECT_EQ(reg_.sp, 0xFE);
}

TEST_F(StackTest, PLA) {
  reg_.sp = 0xFE;
  reg_.acc = 0;
  EXPECT_CALL(bus_, Read(0x01FF, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAB), Return(Peripheral::Status::OK)));
  pla_.Execute(0);
  EXPECT_EQ(reg_.sp, 0xFF);
  EXPECT_EQ(reg_.acc, 0xAB);
}

TEST_F(StackTest, PHP) {
  reg_.sp = 0xFF;
  reg_.status = 0xAB;
  EXPECT_CALL(bus_, Write(0x01FF, 0xAB))
      .WillOnce(Return(Peripheral::Status::OK));
  php_.Execute(0);
  EXPECT_EQ(reg_.sp, 0xFE);
}

TEST_F(StackTest, PLP) {
  reg_.sp = 0xFE;
  reg_.status = 0;
  EXPECT_CALL(bus_, Read(0x01FF, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAB), Return(Peripheral::Status::OK)));
  plp_.Execute(0);
  EXPECT_EQ(reg_.sp, 0xFF);
  EXPECT_EQ(reg_.status, 0xAB);
}

TEST_F(StackTest, Timing) {
  EXPECT_EQ(3, TimeInstruction(&pha_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(4, TimeInstruction(&pla_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(3, TimeInstruction(&php_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(4, TimeInstruction(&plp_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
}

}  // namespace
