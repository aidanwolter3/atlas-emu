#include "src/engine/instruction/transfer.h"

#include "src/engine/instruction/instruction_test_base.h"

namespace {

class TransferTest : public InstructionTestBase {
 protected:
  TXS txs_{bus_, reg_};
  TSX tsx_{bus_, reg_};
};

TEST_F(TransferTest, TXS) {
  reg_.sp = 0;
  reg_.x = 0xAB;
  txs_.Execute(0);
  EXPECT_EQ(reg_.sp, 0xAB);
}

TEST_F(TransferTest, TSX) {
  reg_.sp = 0xAB;
  reg_.x = 0;
  tsx_.Execute(0);
  EXPECT_EQ(reg_.x, 0xAB);
}

TEST_F(TransferTest, Timing) {
  EXPECT_EQ(2, TimeInstruction(&txs_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
  EXPECT_EQ(2, TimeInstruction(&tsx_, Instruction::Mode::kImplied,
                               Instruction::Operation::kNone));
}

}  // namespace
