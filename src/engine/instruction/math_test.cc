#include "src/engine/instruction/math.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class MathTest : public InstructionTestBase {};

TEST_F(MathTest, ADC_NoOverflowNoCarry) {
  reg_.acc = 10;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, false);

  ADC adc(bus_, reg_);
  adc.Execute(10);

  EXPECT_EQ(reg_.acc, 20);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, ADC_OverflowNoCarry) {
  reg_.acc = 80;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, false);

  ADC adc(bus_, reg_);
  adc.Execute(80);

  EXPECT_EQ(reg_.acc, 160);
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, ADC_NoOverflowCarry) {
  reg_.acc = 80;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, false);

  ADC adc(bus_, reg_);
  adc.Execute(208);

  EXPECT_EQ(reg_.acc, 32);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, ADC_OverflowCarry) {
  reg_.acc = 208;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, false);

  ADC adc(bus_, reg_);
  adc.Execute(144);

  EXPECT_EQ(reg_.acc, 96);
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, SBC_NoOverflowNoCarry) {
  reg_.acc = 80;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, true);  // borrow = false

  SBC sbc(bus_, reg_);
  sbc.Execute(240);

  EXPECT_EQ(reg_.acc, 96);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, SBC_OverflowNoCarry) {
  reg_.acc = 80;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, true);  // borrow = false

  SBC sbc(bus_, reg_);
  sbc.Execute(176);

  EXPECT_EQ(reg_.acc, 160);
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, SBC_NoOverflowCarry) {
  reg_.acc = 80;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, true);  // borrow = false

  SBC sbc(bus_, reg_);
  sbc.Execute(48);

  EXPECT_EQ(reg_.acc, 32);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, SBC_OverflowCarry) {
  reg_.acc = 208;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, true);  // borrow = false

  SBC sbc(bus_, reg_);
  sbc.Execute(112);

  EXPECT_EQ(reg_.acc, 96);
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, DEC_ZeroPage) {
  DEC dec(bus_, reg_);
  uint8_t result = dec.Execute(0xAA).data;

  EXPECT_EQ(result, 0xA9);
  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_TRUE(reg_.status.test(Status::kSign));
}

TEST_F(MathTest, INC_ZeroPage) {
  INC inc(bus_, reg_);
  uint8_t result = inc.Execute(0xAA).data;

  EXPECT_EQ(result, 0xAB);
  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_TRUE(reg_.status.test(Status::kSign));
}

}  // namespace
