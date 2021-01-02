#include "src/engine/instruction/jump.h"

#include "src/engine/instruction/instruction_test_base.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

namespace {

class JumpTest : public InstructionTestBase {};

TEST_F(JumpTest, JSR_RTS) {
  reg_.pc = 0x1122;
  reg_.sp = 0x10;
  JSR jsr(bus_, reg_);
  RTS rts(bus_, reg_);

  EXPECT_CALL(bus_, Read(0x1122, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xAA), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x1123, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xBB), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Write(0x110, 0x11));
  EXPECT_CALL(bus_, Write(0x10F, 0x23));

  jsr.Execute(0x20);
  EXPECT_EQ(reg_.pc, 0xBBAA);
  EXPECT_EQ(reg_.sp, 0x0E);

  EXPECT_CALL(bus_, Read(0x10F, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x23), Return(Peripheral::Status::OK)));
  EXPECT_CALL(bus_, Read(0x110, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x11), Return(Peripheral::Status::OK)));

  rts.Execute(0x60);
  EXPECT_EQ(reg_.pc, 0x1124);
  EXPECT_EQ(reg_.sp, 0x10);
}

}  // namespace
