#include "src/instruction/store.h"

#include "src/instruction/instruction_test_base.h"

namespace {

using testing::Return;

class StoreTest : public InstructionTestBase {
 protected:
  void TestZeroPage(Instruction& instruction, uint8_t data, uint8_t opcode);
  void TestIndexedZeroPage(Instruction& instruction, uint8_t data,
                           uint8_t& index, uint8_t opcode);
  void TestAbsolute(Instruction& instruction, uint8_t data, uint8_t opcode);
  void TestIndexedAbsolute(Instruction& instruction, uint8_t data,
                           uint8_t& index, uint8_t opcode);
  void TestIndexedIndirect(Instruction& instruction, uint8_t data,
                           uint8_t& index, uint8_t opcode);
  void TestIndirectIndexed(Instruction& instruction, uint8_t data,
                           uint8_t& index, uint8_t opcode);
};

void StoreTest::TestZeroPage(Instruction& instruction, uint8_t data,
                             uint8_t opcode) {
  auto address = ExpectZeroPage();
  EXPECT_CALL(mem_, Write(address, data)).WillOnce(Return(Memory::Status::OK));
  instruction.Execute(opcode);
  ExpectPostZeroPage();
}

void StoreTest::TestIndexedZeroPage(Instruction& instruction, uint8_t data,
                                    uint8_t& index, uint8_t opcode) {
  auto address = ExpectIndexedZeroPage(index);
  EXPECT_CALL(mem_, Write(address, data)).WillOnce(Return(Memory::Status::OK));
  instruction.Execute(opcode);
  ExpectPostIndexedZeroPage();
}

void StoreTest::TestAbsolute(Instruction& instruction, uint8_t data,
                             uint8_t opcode) {
  auto address = ExpectAbsolute();
  EXPECT_CALL(mem_, Write(address, data)).WillOnce(Return(Memory::Status::OK));
  instruction.Execute(opcode);
  ExpectPostAbsolute();
}

void StoreTest::TestIndexedAbsolute(Instruction& instruction, uint8_t data,
                                    uint8_t& index, uint8_t opcode) {
  auto address = ExpectIndexedAbsolute(index);
  EXPECT_CALL(mem_, Write(address, data)).WillOnce(Return(Memory::Status::OK));
  instruction.Execute(opcode);
  ExpectPostIndexedAbsolute();
}

void StoreTest::TestIndexedIndirect(Instruction& instruction, uint8_t data,
                                    uint8_t& index, uint8_t opcode) {
  auto address = ExpectIndexedIndirect(index);
  EXPECT_CALL(mem_, Write(address, data)).WillOnce(Return(Memory::Status::OK));
  instruction.Execute(opcode);
  ExpectPostIndexedIndirect();
}

void StoreTest::TestIndirectIndexed(Instruction& instruction, uint8_t data,
                                    uint8_t& index, uint8_t opcode) {
  auto address = ExpectIndirectIndexed(index);
  EXPECT_CALL(mem_, Write(address, data)).WillOnce(Return(Memory::Status::OK));
  instruction.Execute(opcode);
  ExpectPostIndirectIndexed();
}

TEST_F(StoreTest, STA) {
  STA sta(mem_, reg_);
  TestZeroPage(sta, reg_.acc, 0x85);
  TestIndexedZeroPage(sta, reg_.acc, reg_.x, 0x95);
  TestAbsolute(sta, reg_.acc, 0x8D);
  TestIndexedAbsolute(sta, reg_.acc, reg_.x, 0x9D);
  TestIndexedAbsolute(sta, reg_.acc, reg_.y, 0x99);
  TestIndexedIndirect(sta, reg_.acc, reg_.x, 0x81);
  TestIndirectIndexed(sta, reg_.acc, reg_.y, 0x91);
}

TEST_F(StoreTest, STX) {
  STX stx(mem_, reg_);
  TestZeroPage(stx, reg_.x, 0x86);
  TestIndexedZeroPage(stx, reg_.x, reg_.y, 0x96);
  TestAbsolute(stx, reg_.x, 0x8E);
}

TEST_F(StoreTest, STY) {
  STY sty(mem_, reg_);
  TestZeroPage(sty, reg_.y, 0x84);
  TestIndexedZeroPage(sty, reg_.y, reg_.x, 0x94);
  TestAbsolute(sty, reg_.y, 0x8C);
}

}  // namespace
