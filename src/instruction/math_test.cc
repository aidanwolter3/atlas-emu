#include "src/instruction/math.h"

#include "src/instruction/instruction_test_base.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

namespace {

class MathTest : public InstructionTestBase {};

TEST_F(MathTest, ADC_NoOverflowNoCarry) {
  reg_.acc = 10;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, false);
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(10), Return(Peripheral::Status::OK)));

  ADC adc(bus_, reg_);
  adc.Execute(0x69);

  EXPECT_EQ(reg_.acc, 20);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, ADC_OverflowNoCarry) {
  reg_.acc = 80;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, false);
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(80), Return(Peripheral::Status::OK)));

  ADC adc(bus_, reg_);
  adc.Execute(0x69);

  EXPECT_EQ(reg_.acc, 160);
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, ADC_NoOverflowCarry) {
  reg_.acc = 80;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, false);
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(208), Return(Peripheral::Status::OK)));

  ADC adc(bus_, reg_);
  adc.Execute(0x69);

  EXPECT_EQ(reg_.acc, 32);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, ADC_OverflowCarry) {
  reg_.acc = 208;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, false);
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(144), Return(Peripheral::Status::OK)));

  ADC adc(bus_, reg_);
  adc.Execute(0x69);

  EXPECT_EQ(reg_.acc, 96);
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, SBC_NoOverflowNoCarry) {
  reg_.acc = 80;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, true);  // borrow = false
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(240), Return(Peripheral::Status::OK)));

  SBC sbc(bus_, reg_);
  sbc.Execute(0xE9);

  EXPECT_EQ(reg_.acc, 96);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, SBC_OverflowNoCarry) {
  reg_.acc = 80;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, true);  // borrow = false
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(176), Return(Peripheral::Status::OK)));

  SBC sbc(bus_, reg_);
  sbc.Execute(0xE9);

  EXPECT_EQ(reg_.acc, 160);
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));
  EXPECT_FALSE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, SBC_NoOverflowCarry) {
  reg_.acc = 80;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, true);  // borrow = false
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(48), Return(Peripheral::Status::OK)));

  SBC sbc(bus_, reg_);
  sbc.Execute(0xE9);

  EXPECT_EQ(reg_.acc, 32);
  EXPECT_FALSE(reg_.status.test(Status::kOverflow));
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

TEST_F(MathTest, SBC_OverflowCarry) {
  reg_.acc = 208;
  reg_.pc = 0;
  reg_.status.set(Status::kCarry, true);  // borrow = false
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(112), Return(Peripheral::Status::OK)));

  SBC sbc(bus_, reg_);
  sbc.Execute(0xE9);

  EXPECT_EQ(reg_.acc, 96);
  EXPECT_TRUE(reg_.status.test(Status::kOverflow));
  EXPECT_TRUE(reg_.status.test(Status::kCarry));
}

}  // namespace
