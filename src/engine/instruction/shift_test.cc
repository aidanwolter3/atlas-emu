#include "src/engine/instruction/shift.h"

#include <cstdint>

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class ShiftTest : public InstructionTestBase {
 protected:
  void ExpectSignZeroCarry(bool sign, bool zero, bool carry) {
    EXPECT_EQ(sign, reg_.status.test(Status::kSign));
    EXPECT_EQ(zero, reg_.status.test(Status::kZero));
    EXPECT_EQ(carry, reg_.status.test(Status::kCarry));
  }

  ASL asl_{bus_, reg_};
  LSR lsr_{bus_, reg_};
  ROL rol_{bus_, reg_};
  ROR ror_{bus_, reg_};
};

TEST_F(ShiftTest, ASL) {
  uint8_t result = 0b01100101;

  result = asl_.Execute(result).data;
  EXPECT_EQ(result, 0b11001010);
  ExpectSignZeroCarry(true, false, false);

  result = asl_.Execute(result).data;
  EXPECT_EQ(result, 0b10010100);
  ExpectSignZeroCarry(true, false, true);

  result = asl_.Execute(result).data;
  EXPECT_EQ(result, 0b00101000);
  ExpectSignZeroCarry(false, false, true);

  for (int i = 0; i < 5; ++i) {
    result = asl_.Execute(result).data;
  }
  EXPECT_EQ(result, 0b00000000);
  ExpectSignZeroCarry(false, true, true);

  result = asl_.Execute(result).data;
  EXPECT_EQ(result, 0b00000000);
  ExpectSignZeroCarry(false, true, false);
}

TEST_F(ShiftTest, LSR_Accumulator) {
  uint8_t result = 0b01100101;

  result = lsr_.Execute(result).data;
  EXPECT_EQ(result, 0b00110010);
  ExpectSignZeroCarry(false, false, true);

  result = lsr_.Execute(result).data;
  EXPECT_EQ(result, 0b00011001);
  ExpectSignZeroCarry(false, false, false);

  result = lsr_.Execute(result).data;
  EXPECT_EQ(result, 0b00001100);
  ExpectSignZeroCarry(false, false, true);

  for (int i = 0; i < 4; ++i) {
    result = lsr_.Execute(result).data;
  }
  EXPECT_EQ(result, 0b00000000);
  ExpectSignZeroCarry(false, true, true);

  result = lsr_.Execute(result).data;
  EXPECT_EQ(result, 0b00000000);
  ExpectSignZeroCarry(false, true, false);
}

TEST_F(ShiftTest, ROL_Accumulator) {
  uint8_t result = 0b01100101;

  result = rol_.Execute(result).data;
  EXPECT_EQ(result, 0b11001010);
  ExpectSignZeroCarry(true, false, false);

  result = rol_.Execute(result).data;
  EXPECT_EQ(result, 0b10010100);
  ExpectSignZeroCarry(true, false, true);

  result = rol_.Execute(result).data;
  EXPECT_EQ(result, 0b00101001);
  ExpectSignZeroCarry(false, false, true);

  for (int i = 0; i < 5; ++i) {
    result = rol_.Execute(result).data;
  }
  EXPECT_EQ(result, 0b00110010);
  ExpectSignZeroCarry(false, false, true);

  result = rol_.Execute(result).data;
  EXPECT_EQ(result, 0b01100101);
  ExpectSignZeroCarry(false, false, false);
}

TEST_F(ShiftTest, ROR_Accumulator) {
  uint8_t result = 0b01100101;

  result = ror_.Execute(result).data;
  EXPECT_EQ(result, 0b00110010);
  ExpectSignZeroCarry(false, false, true);

  result = ror_.Execute(result).data;
  EXPECT_EQ(result, 0b10011001);
  ExpectSignZeroCarry(true, false, false);

  result = ror_.Execute(result).data;
  EXPECT_EQ(result, 0b01001100);
  ExpectSignZeroCarry(false, false, true);

  for (int i = 0; i < 4; ++i) {
    result = ror_.Execute(result).data;
  }
  EXPECT_EQ(result, 0b10010100);
  ExpectSignZeroCarry(true, false, true);

  result = ror_.Execute(result).data;
  EXPECT_EQ(result, 0b11001010);
  ExpectSignZeroCarry(true, false, false);

  result = ror_.Execute(result).data;
  EXPECT_EQ(result, 0b01100101);
  ExpectSignZeroCarry(false, false, false);
}

TEST_F(ShiftTest, Timing) {
  EXPECT_EQ(2, TimeInstruction(&asl_, Instruction::Mode::kImplied,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(5, TimeInstruction(&asl_, Instruction::Mode::kZeroPage,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(6, TimeInstruction(&asl_, Instruction::Mode::kZeroPageX,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(6, TimeInstruction(&asl_, Instruction::Mode::kAbsolute,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(7, TimeInstruction(&asl_, Instruction::Mode::kAbsoluteX,
                               Instruction::Operation::kReadWrite));

  EXPECT_EQ(2, TimeInstruction(&lsr_, Instruction::Mode::kImplied,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(5, TimeInstruction(&lsr_, Instruction::Mode::kZeroPage,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(6, TimeInstruction(&lsr_, Instruction::Mode::kZeroPageX,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(6, TimeInstruction(&lsr_, Instruction::Mode::kAbsolute,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(7, TimeInstruction(&lsr_, Instruction::Mode::kAbsoluteX,
                               Instruction::Operation::kReadWrite));

  EXPECT_EQ(2, TimeInstruction(&rol_, Instruction::Mode::kImplied,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(5, TimeInstruction(&rol_, Instruction::Mode::kZeroPage,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(6, TimeInstruction(&rol_, Instruction::Mode::kZeroPageX,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(6, TimeInstruction(&rol_, Instruction::Mode::kAbsolute,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(7, TimeInstruction(&rol_, Instruction::Mode::kAbsoluteX,
                               Instruction::Operation::kReadWrite));

  EXPECT_EQ(2, TimeInstruction(&ror_, Instruction::Mode::kImplied,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(5, TimeInstruction(&ror_, Instruction::Mode::kZeroPage,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(6, TimeInstruction(&ror_, Instruction::Mode::kZeroPageX,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(6, TimeInstruction(&ror_, Instruction::Mode::kAbsolute,
                               Instruction::Operation::kReadWrite));
  EXPECT_EQ(7, TimeInstruction(&ror_, Instruction::Mode::kAbsoluteX,
                               Instruction::Operation::kReadWrite));
}

}  // namespace
