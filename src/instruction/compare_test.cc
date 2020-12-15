#include "src/instruction/compare.h"

#include "src/instruction/instruction_test_base.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

namespace {

class CompareTest : public InstructionTestBase {
 protected:
  void ExpectZeroCarrySign(bool zero, bool carry, bool sign) {
    EXPECT_EQ(zero, reg_.status.test(Status::kZero));
    EXPECT_EQ(carry, reg_.status.test(Status::kCarry));
    EXPECT_EQ(sign, reg_.status.test(Status::kSign));
  }
};

TEST_F(CompareTest, CMP_Immediate) {
  reg_.acc = 10;
  CMP cmp(bus_, reg_);

  reg_.pc = 0;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(10), Return(Peripheral::Status::OK)));
  cmp.Execute(0xC9);
  ExpectZeroCarrySign(true, true, false);

  reg_.pc = 0;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(20), Return(Peripheral::Status::OK)));
  cmp.Execute(0xC9);
  ExpectZeroCarrySign(false, false, true);

  reg_.pc = 0;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0), Return(Peripheral::Status::OK)));
  cmp.Execute(0xC9);
  ExpectZeroCarrySign(false, true, false);
}

TEST_F(CompareTest, CMP_ZeroPage) {
  reg_.acc = 10;
  CMP cmp(bus_, reg_);

  reg_.pc = 0;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAA), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0xAA, _))
      .WillOnce(DoAll(SetArgPointee<1>(10), Return(Peripheral::Status::OK)));
  cmp.Execute(0xC5);
  ExpectZeroCarrySign(true, true, false);

  reg_.pc = 0;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAA), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0xAA, _))
      .WillOnce(DoAll(SetArgPointee<1>(20), Return(Peripheral::Status::OK)));
  cmp.Execute(0xC5);
  ExpectZeroCarrySign(false, false, true);

  reg_.pc = 0;
  EXPECT_CALL(bus_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAA), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0xAA, _))
      .WillOnce(DoAll(SetArgPointee<1>(0), Return(Peripheral::Status::OK)));
  cmp.Execute(0xC5);
  ExpectZeroCarrySign(false, true, false);
}

}  // namespace
