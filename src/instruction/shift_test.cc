#include "src/instruction/shift.h"

#include "src/instruction/instruction_test_base.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

namespace {

class ShiftTest : public InstructionTestBase {
 protected:
  void ExpectSignZeroCarry(bool sign, bool zero, bool carry) {
    EXPECT_EQ(sign, reg_.status.test(Status::kSign));
    EXPECT_EQ(zero, reg_.status.test(Status::kZero));
    EXPECT_EQ(carry, reg_.status.test(Status::kCarry));
  }

  void ExpectReadWrite(uint8_t read, uint8_t write) {
    reg_.pc = 0;
    EXPECT_CALL(bus_, Read(0, _))
        .WillOnce(
            DoAll(SetArgPointee<1>(0xAB), Return(Peripheral::Status::OK)));
    EXPECT_CALL(bus_, Read(0xAB, _))
        .WillOnce(
            DoAll(SetArgPointee<1>(read), Return(Peripheral::Status::OK)));
    EXPECT_CALL(bus_, Write(0xAB, write));
  }
};

TEST_F(ShiftTest, ASL_Accumulator) {
  reg_.acc = 0b01100101;
  ASL asl(bus_, reg_);

  asl.Execute(0x0A);
  EXPECT_EQ(reg_.acc, 0b11001010);
  ExpectSignZeroCarry(true, false, false);

  asl.Execute(0x0A);
  EXPECT_EQ(reg_.acc, 0b10010100);
  ExpectSignZeroCarry(true, false, true);

  asl.Execute(0x0A);
  EXPECT_EQ(reg_.acc, 0b00101000);
  ExpectSignZeroCarry(false, false, true);

  asl.Execute(0x0A);
  asl.Execute(0x0A);
  asl.Execute(0x0A);
  asl.Execute(0x0A);
  asl.Execute(0x0A);
  EXPECT_EQ(reg_.acc, 0b00000000);
  ExpectSignZeroCarry(false, true, true);

  asl.Execute(0x0A);
  EXPECT_EQ(reg_.acc, 0b00000000);
  ExpectSignZeroCarry(false, true, false);
}

TEST_F(ShiftTest, ASL_Immediate) {
  ASL asl(bus_, reg_);

  ExpectReadWrite(0b01100101, 0b11001010);
  asl.Execute(0x06);
  ExpectSignZeroCarry(true, false, false);

  ExpectReadWrite(0b11001010, 0b10010100);
  asl.Execute(0x06);
  ExpectSignZeroCarry(true, false, true);

  ExpectReadWrite(0b10010100, 0b00101000);
  asl.Execute(0x06);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b00101000, 0b01010000);
  asl.Execute(0x06);
  ExpectSignZeroCarry(false, false, false);

  ExpectReadWrite(0b01010000, 0b10100000);
  asl.Execute(0x06);
  ExpectSignZeroCarry(true, false, false);

  ExpectReadWrite(0b10100000, 0b01000000);
  asl.Execute(0x06);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b01000000, 0b10000000);
  asl.Execute(0x06);
  ExpectSignZeroCarry(true, false, false);

  ExpectReadWrite(0b10000000, 0b00000000);
  asl.Execute(0x06);
  ExpectSignZeroCarry(false, true, true);
}

TEST_F(ShiftTest, LSR_Accumulator) {
  reg_.acc = 0b01100101;
  LSR lsr(bus_, reg_);

  lsr.Execute(0x4A);
  EXPECT_EQ(reg_.acc, 0b00110010);
  ExpectSignZeroCarry(false, false, true);

  lsr.Execute(0x4A);
  EXPECT_EQ(reg_.acc, 0b00011001);
  ExpectSignZeroCarry(false, false, false);

  lsr.Execute(0x4A);
  EXPECT_EQ(reg_.acc, 0b00001100);
  ExpectSignZeroCarry(false, false, true);

  lsr.Execute(0x4A);
  lsr.Execute(0x4A);
  lsr.Execute(0x4A);
  lsr.Execute(0x4A);
  EXPECT_EQ(reg_.acc, 0b00000000);
  ExpectSignZeroCarry(false, true, true);

  lsr.Execute(0x4A);
  EXPECT_EQ(reg_.acc, 0b00000000);
  ExpectSignZeroCarry(false, true, false);
}

TEST_F(ShiftTest, LSR_Immediate) {
  LSR lsr(bus_, reg_);

  ExpectReadWrite(0b01100101, 0b00110010);
  lsr.Execute(0x46);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b00110010, 0b00011001);
  lsr.Execute(0x46);
  ExpectSignZeroCarry(false, false, false);

  ExpectReadWrite(0b00011001, 0b00001100);
  lsr.Execute(0x46);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b00001100, 0b00000110);
  lsr.Execute(0x46);
  ExpectSignZeroCarry(false, false, false);

  ExpectReadWrite(0b00000110, 0b00000011);
  lsr.Execute(0x46);
  ExpectSignZeroCarry(false, false, false);

  ExpectReadWrite(0b00000011, 0b00000001);
  lsr.Execute(0x46);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b00000001, 0b00000000);
  lsr.Execute(0x46);
  ExpectSignZeroCarry(false, true, true);

  ExpectReadWrite(0b00000000, 0b00000000);
  lsr.Execute(0x46);
  ExpectSignZeroCarry(false, true, false);
}

TEST_F(ShiftTest, ROL_Accumulator) {
  reg_.acc = 0b01100101;
  ROL rol(bus_, reg_);

  rol.Execute(0x2A);
  EXPECT_EQ(reg_.acc, 0b11001010);
  ExpectSignZeroCarry(true, false, false);

  rol.Execute(0x2A);
  EXPECT_EQ(reg_.acc, 0b10010100);
  ExpectSignZeroCarry(true, false, true);

  rol.Execute(0x2A);
  EXPECT_EQ(reg_.acc, 0b00101001);
  ExpectSignZeroCarry(false, false, true);

  rol.Execute(0x2A);
  rol.Execute(0x2A);
  rol.Execute(0x2A);
  rol.Execute(0x2A);
  rol.Execute(0x2A);
  EXPECT_EQ(reg_.acc, 0b00110010);
  ExpectSignZeroCarry(false, false, true);

  rol.Execute(0x2A);
  EXPECT_EQ(reg_.acc, 0b01100101);
  ExpectSignZeroCarry(false, false, false);
}

TEST_F(ShiftTest, ROL_Immediate) {
  ROL rol(bus_, reg_);

  ExpectReadWrite(0b01100101, 0b11001010);
  rol.Execute(0x26);
  ExpectSignZeroCarry(true, false, false);

  ExpectReadWrite(0b11001010, 0b10010100);
  rol.Execute(0x26);
  ExpectSignZeroCarry(true, false, true);

  ExpectReadWrite(0b10010100, 0b00101001);
  rol.Execute(0x26);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b00101001, 0b01010011);
  rol.Execute(0x26);
  ExpectSignZeroCarry(false, false, false);

  ExpectReadWrite(0b01010011, 0b10100110);
  rol.Execute(0x26);
  ExpectSignZeroCarry(true, false, false);

  ExpectReadWrite(0b10100110, 0b01001100);
  rol.Execute(0x26);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b01001100, 0b10011001);
  rol.Execute(0x26);
  ExpectSignZeroCarry(true, false, false);

  ExpectReadWrite(0b10011001, 0b00110010);
  rol.Execute(0x26);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b00110010, 0b01100101);
  rol.Execute(0x26);
  ExpectSignZeroCarry(false, false, false);
}

TEST_F(ShiftTest, ROR_Accumulator) {
  reg_.acc = 0b01100101;
  ROR ror(bus_, reg_);

  ror.Execute(0x6A);
  EXPECT_EQ(reg_.acc, 0b00110010);
  ExpectSignZeroCarry(false, false, true);

  ror.Execute(0x6A);
  EXPECT_EQ(reg_.acc, 0b10011001);
  ExpectSignZeroCarry(true, false, false);

  ror.Execute(0x6A);
  EXPECT_EQ(reg_.acc, 0b01001100);
  ExpectSignZeroCarry(false, false, true);

  ror.Execute(0x6A);
  ror.Execute(0x6A);
  ror.Execute(0x6A);
  ror.Execute(0x6A);
  EXPECT_EQ(reg_.acc, 0b10010100);
  ExpectSignZeroCarry(true, false, true);

  ror.Execute(0x6A);
  EXPECT_EQ(reg_.acc, 0b11001010);
  ExpectSignZeroCarry(true, false, false);

  ror.Execute(0x6A);
  EXPECT_EQ(reg_.acc, 0b01100101);
  ExpectSignZeroCarry(false, false, false);
}

TEST_F(ShiftTest, ROR_Immediate) {
  ROR ror(bus_, reg_);

  ExpectReadWrite(0b01100101, 0b00110010);
  ror.Execute(0x66);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b00110010, 0b10011001);
  ror.Execute(0x66);
  ExpectSignZeroCarry(true, false, false);

  ExpectReadWrite(0b10011001, 0b01001100);
  ror.Execute(0x66);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b01001100, 0b10100110);
  ror.Execute(0x66);
  ExpectSignZeroCarry(true, false, false);

  ExpectReadWrite(0b10100110, 0b01010011);
  ror.Execute(0x66);
  ExpectSignZeroCarry(false, false, false);

  ExpectReadWrite(0b01010011, 0b00101001);
  ror.Execute(0x66);
  ExpectSignZeroCarry(false, false, true);

  ExpectReadWrite(0b00101001, 0b10010100);
  ror.Execute(0x66);
  ExpectSignZeroCarry(true, false, true);

  ExpectReadWrite(0b10010100, 0b11001010);
  ror.Execute(0x66);
  ExpectSignZeroCarry(true, false, false);

  ExpectReadWrite(0b11001010, 0b01100101);
  ror.Execute(0x66);
  ExpectSignZeroCarry(false, false, false);
}

}  // namespace
