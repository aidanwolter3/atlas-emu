#include "src/engine/instruction/transfer.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class TransferTest : public Instruction2TestBase {};

TEST_F(TransferTest, TXS) {
  reg_.sp = 0;
  reg_.x = 0xAB;
  TXS txs(bus_, reg_);
  int cycles = ExecuteUntilComplete(&txs);
  EXPECT_EQ(cycles, 2);
  EXPECT_EQ(reg_.sp, 0xAB);
}

TEST_F(TransferTest, TSX) {
  reg_.sp = 0xAB;
  reg_.x = 0;
  TSX tsx(bus_, reg_);
  int cycles = ExecuteUntilComplete(&tsx);
  EXPECT_EQ(cycles, 2);
  EXPECT_EQ(reg_.x, 0xAB);
}

}  // namespace
