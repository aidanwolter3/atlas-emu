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
};

TEST_F(ShiftTest, ASL) {
  uint8_t result = 0b01100101;
  ASL asl(bus_, reg_);

  result = asl.Execute(result);
  EXPECT_EQ(result, 0b11001010);
  ExpectSignZeroCarry(true, false, false);

  result = asl.Execute(result);
  EXPECT_EQ(result, 0b10010100);
  ExpectSignZeroCarry(true, false, true);

  result = asl.Execute(result);
  EXPECT_EQ(result, 0b00101000);
  ExpectSignZeroCarry(false, false, true);

  for (int i = 0; i < 5; ++i) {
    result = asl.Execute(result);
  }
  EXPECT_EQ(result, 0b00000000);
  ExpectSignZeroCarry(false, true, true);

  result = asl.Execute(result);
  EXPECT_EQ(result, 0b00000000);
  ExpectSignZeroCarry(false, true, false);
}

TEST_F(ShiftTest, LSR_Accumulator) {
  uint8_t result = 0b01100101;
  LSR lsr(bus_, reg_);

  result = lsr.Execute(result);
  EXPECT_EQ(result, 0b00110010);
  ExpectSignZeroCarry(false, false, true);

  result = lsr.Execute(result);
  EXPECT_EQ(result, 0b00011001);
  ExpectSignZeroCarry(false, false, false);

  result = lsr.Execute(result);
  EXPECT_EQ(result, 0b00001100);
  ExpectSignZeroCarry(false, false, true);

  for (int i = 0; i < 4; ++i) {
    result = lsr.Execute(result);
  }
  EXPECT_EQ(result, 0b00000000);
  ExpectSignZeroCarry(false, true, true);

  result = lsr.Execute(result);
  EXPECT_EQ(result, 0b00000000);
  ExpectSignZeroCarry(false, true, false);
}

TEST_F(ShiftTest, ROL_Accumulator) {
  uint8_t result = 0b01100101;
  ROL rol(bus_, reg_);

  result = rol.Execute(result);
  EXPECT_EQ(result, 0b11001010);
  ExpectSignZeroCarry(true, false, false);

  result = rol.Execute(result);
  EXPECT_EQ(result, 0b10010100);
  ExpectSignZeroCarry(true, false, true);

  result = rol.Execute(result);
  EXPECT_EQ(result, 0b00101001);
  ExpectSignZeroCarry(false, false, true);

  for (int i = 0; i < 5; ++i) {
    result = rol.Execute(result);
  }
  EXPECT_EQ(result, 0b00110010);
  ExpectSignZeroCarry(false, false, true);

  result = rol.Execute(result);
  EXPECT_EQ(result, 0b01100101);
  ExpectSignZeroCarry(false, false, false);
}

TEST_F(ShiftTest, ROR_Accumulator) {
  uint8_t result = 0b01100101;
  ROR ror(bus_, reg_);

  result = ror.Execute(result);
  EXPECT_EQ(result, 0b00110010);
  ExpectSignZeroCarry(false, false, true);

  result = ror.Execute(result);
  EXPECT_EQ(result, 0b10011001);
  ExpectSignZeroCarry(true, false, false);

  result = ror.Execute(result);
  EXPECT_EQ(result, 0b01001100);
  ExpectSignZeroCarry(false, false, true);

  for (int i = 0; i < 4; ++i) {
    result = ror.Execute(result);
  }
  EXPECT_EQ(result, 0b10010100);
  ExpectSignZeroCarry(true, false, true);

  result = ror.Execute(result);
  EXPECT_EQ(result, 0b11001010);
  ExpectSignZeroCarry(true, false, false);

  result = ror.Execute(result);
  EXPECT_EQ(result, 0b01100101);
  ExpectSignZeroCarry(false, false, false);
}

}  // namespace
