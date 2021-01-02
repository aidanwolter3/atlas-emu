#include "src/engine/instruction/transfer.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class TransferTest : public InstructionTestBase {};

TEST_F(TransferTest, TXS) {
  reg_.sp = 0;
  reg_.x = 0xAB;
  TXS txs(bus_, reg_);
  txs.Execute(0x9A);
  EXPECT_EQ(reg_.sp, 0xAB);
}

TEST_F(TransferTest, TSX) {
  reg_.sp = 0xAB;
  reg_.x = 0;
  TSX tsx(bus_, reg_);
  tsx.Execute(0xBA);
  EXPECT_EQ(reg_.x, 0xAB);
}

}  // namespace
