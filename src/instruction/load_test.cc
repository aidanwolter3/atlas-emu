#include "src/instruction/load.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/public/memory.h"
#include "src/public/registers.h"

namespace {

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;

class MockMemory : public Memory {
 public:
  MOCK_METHOD2(Read, Memory::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Memory::Status(uint16_t address, uint8_t data));
};

class LoadTest : public testing::Test {
 protected:
  void TestImmediate(Load& load, uint8_t& dest, uint8_t opcode);
  void TestZeroPage(Load& load, uint8_t& dest, uint8_t opcode);
  void TestIndexedZeroPage(Load& load, uint8_t& dest, uint8_t& index,
                           uint8_t opcode);
  void TestAbsolute(Load& load, uint8_t& dest, uint8_t opcode);
  void TestIndexedAbsolute(Load& load, uint8_t& dest, uint8_t& index,
                           uint8_t opcode);
  void TestIndexedIndirect(Load& load, uint8_t& dest, uint8_t& index,
                           uint8_t opcode);
  void TestIndirectIndexed(Load& load, uint8_t& dest, uint8_t& index,
                           uint8_t opcode);
  void TestStatusZero(Load& load, uint8_t immediate_opcode);
  void TestStatusNegative(Load& load, uint8_t immediate_opcode);

  MockMemory mem_;
  Registers reg_;
};

void LoadTest::TestImmediate(Load& load, uint8_t& dest, uint8_t opcode) {
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  load.Execute(opcode);

  EXPECT_EQ(reg_.pc, 0x01);
  EXPECT_EQ(dest, 0x12);
}

void LoadTest::TestZeroPage(Load& load, uint8_t& dest, uint8_t opcode) {
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x12, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  load.Execute(opcode);

  EXPECT_EQ(reg_.pc, 0x01);
  EXPECT_EQ(dest, 0x34);
}

void LoadTest::TestIndexedZeroPage(Load& load, uint8_t& dest, uint8_t& index,
                                   uint8_t opcode) {
  reg_.pc = 0x00;
  index = 7;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x19, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  load.Execute(opcode);

  EXPECT_EQ(reg_.pc, 0x01);
  EXPECT_EQ(dest, 0x34);
}

void LoadTest::TestAbsolute(Load& load, uint8_t& dest, uint8_t opcode) {
  reg_.pc = 0;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x01, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x1234, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x56), Return(Memory::Status::OK)));
  load.Execute(opcode);

  EXPECT_EQ(reg_.pc, 0x02);
  EXPECT_EQ(dest, 0x56);
}

void LoadTest::TestIndexedAbsolute(Load& load, uint8_t& dest, uint8_t& index,
                                   uint8_t opcode) {
  reg_.pc = 0;
  index = 7;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x01, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x123B, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x56), Return(Memory::Status::OK)));
  load.Execute(opcode);

  EXPECT_EQ(reg_.pc, 0x02);
  EXPECT_EQ(dest, 0x56);
}

void LoadTest::TestIndexedIndirect(Load& load, uint8_t& dest, uint8_t& index,
                                   uint8_t opcode) {
  reg_.pc = 0;
  index = 7;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x19, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x34, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x56), Return(Memory::Status::OK)));
  load.Execute(opcode);

  EXPECT_EQ(reg_.pc, 0x01);
  EXPECT_EQ(dest, 0x56);
}

void LoadTest::TestIndirectIndexed(Load& load, uint8_t& dest, uint8_t& index,
                                   uint8_t opcode) {
  reg_.pc = 0;
  index = 7;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x12, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  EXPECT_CALL(mem_, Read(0x3B, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x56), Return(Memory::Status::OK)));
  load.Execute(opcode);

  EXPECT_EQ(reg_.pc, 0x01);
  EXPECT_EQ(dest, 0x56);
}

void LoadTest::TestStatusZero(Load& load, uint8_t immediate_opcode) {
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x00), Return(Memory::Status::OK)));
  load.Execute(immediate_opcode);

  EXPECT_TRUE(reg_.status.test(Status::kZero));
  EXPECT_FALSE(reg_.status.test(Status::kSign));
}

void LoadTest::TestStatusNegative(Load& load, uint8_t immediate_opcode) {
  reg_.pc = 0x00;
  EXPECT_CALL(mem_, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xFF), Return(Memory::Status::OK)));
  load.Execute(immediate_opcode);

  EXPECT_FALSE(reg_.status.test(Status::kZero));
  EXPECT_TRUE(reg_.status.test(Status::kSign));
}

TEST_F(LoadTest, LDA) {
  LDA lda(mem_, reg_);
  TestImmediate(lda, reg_.acc, 0xA9);
  TestZeroPage(lda, reg_.acc, 0xA5);
  TestIndexedZeroPage(lda, reg_.acc, reg_.x, 0xB5);
  TestAbsolute(lda, reg_.acc, 0xAD);
  TestIndexedAbsolute(lda, reg_.acc, reg_.x, 0xBD);
  TestIndexedAbsolute(lda, reg_.acc, reg_.y, 0xB9);
  TestIndexedIndirect(lda, reg_.acc, reg_.x, 0xA1);
  TestIndirectIndexed(lda, reg_.acc, reg_.y, 0xB1);
  TestStatusZero(lda, 0xA9);
  TestStatusNegative(lda, 0xA9);
}

TEST_F(LoadTest, LDX) {
  LDX ldx(mem_, reg_);
  TestImmediate(ldx, reg_.x, 0xA2);
  TestZeroPage(ldx, reg_.x, 0xA6);
  TestIndexedZeroPage(ldx, reg_.x, reg_.y, 0xB6);
  TestAbsolute(ldx, reg_.x, 0xAE);
  TestIndexedAbsolute(ldx, reg_.x, reg_.y, 0xBE);
  TestStatusZero(ldx, 0xA2);
  TestStatusNegative(ldx, 0xA2);
}

TEST_F(LoadTest, LDY) {
  LDY ldy(mem_, reg_);
  TestImmediate(ldy, reg_.y, 0xA0);
  TestZeroPage(ldy, reg_.y, 0xA4);
  TestIndexedZeroPage(ldy, reg_.y, reg_.x, 0xB4);
  TestAbsolute(ldy, reg_.y, 0xAC);
  TestIndexedAbsolute(ldy, reg_.y, reg_.x, 0xBC);
  TestStatusZero(ldy, 0xA0);
  TestStatusNegative(ldy, 0xA0);
}

}  // namespace
