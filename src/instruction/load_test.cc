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

TEST(LoadTest, Immediate) {
  MockMemory mem;
  Registers reg;
  LDA lda(mem, reg);

  reg.pc = 0x00;
  EXPECT_CALL(mem, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  lda.Execute(0xA9);

  EXPECT_EQ(reg.pc, 0x01);
  EXPECT_EQ(reg.acc, 0x12);
  EXPECT_FALSE(reg.status.test(Status::kZero));
  EXPECT_FALSE(reg.status.test(Status::kSign));
}

TEST(LoadTest, ZeroPage) {
  MockMemory mem;
  Registers reg;
  LDA lda(mem, reg);

  reg.pc = 0;
  EXPECT_CALL(mem, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x12), Return(Memory::Status::OK)));
  EXPECT_CALL(mem, Read(0x12, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x34), Return(Memory::Status::OK)));
  lda.Execute(0xA5);

  EXPECT_EQ(reg.pc, 0x01);
  EXPECT_EQ(reg.acc, 0x34);
  EXPECT_FALSE(reg.status.test(Status::kZero));
  EXPECT_FALSE(reg.status.test(Status::kSign));
}

TEST(LoadTest, StatusZero) {
  MockMemory mem;
  Registers reg;
  LDA lda(mem, reg);

  reg.pc = 0;
  EXPECT_CALL(mem, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0x00), Return(Memory::Status::OK)));
  lda.Execute(0xA9);

  EXPECT_EQ(reg.pc, 0x01);
  EXPECT_EQ(reg.acc, 0x00);
  EXPECT_TRUE(reg.status.test(Status::kZero));
  EXPECT_FALSE(reg.status.test(Status::kSign));
}

TEST(LoadTest, StatusNegative) {
  MockMemory mem;
  Registers reg;
  LDA lda(mem, reg);

  reg.pc = 0;
  EXPECT_CALL(mem, Read(0x00, _))
      .WillOnce(DoAll(SetArgPointee<1>(0xFF), Return(Memory::Status::OK)));
  lda.Execute(0xA9);

  EXPECT_EQ(reg.pc, 0x01);
  EXPECT_EQ(reg.acc, 0xFF);
  EXPECT_FALSE(reg.status.test(Status::kZero));
  EXPECT_TRUE(reg.status.test(Status::kSign));
}

}  // namespace
