#include <cstdint>
#include <vector>

#include "gtest/gtest.h"
#include "memory_impl.h"

namespace {

const std::vector<uint8_t> kTestData = {
    // Header
    'N',
    'E',
    'S',
    0x1a,
    1,  // PRG count
    0,  // CHR count
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,  // Flags

    // Code
    0x01,
    0x02,
    0x03,
    0x04,
};

std::vector<uint8_t> GenerateLargeTestData(uint8_t prg_count) {
  std::vector<uint8_t> data(0x4010, 0xFF);
  data[4] = prg_count;
  data[5] = 0;  // CHR count
  data[0x10] = 0xAA;
  data[0x400F] = 0xBB;
  return data;
}

TEST(MemoryTest, ReadFirstByte) {
  uint8_t byte_read;
  MemoryImpl mem(GenerateLargeTestData(1));
  auto status = mem.Read(0x8000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);
}

TEST(MemoryTest, ReadLastByte) {
  uint8_t byte_read;
  MemoryImpl mem(GenerateLargeTestData(1));
  auto status = mem.Read(0xFFFF, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xBB);
}

TEST(MemoryTest, ReadOverflowIntoSecondPRG) {
  uint8_t byte_read;
  MemoryImpl mem(GenerateLargeTestData(1));
  auto status = mem.Read(0xC000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);
}

TEST(MemoryTest, ReadOutOfBounds) {
  uint8_t byte_read;
  MemoryImpl mem(kTestData);
  auto status = mem.Read(0x7FFF, &byte_read);
  EXPECT_EQ(status, Memory::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, ReadInvalidPointer) {
  MemoryImpl mem(kTestData);
  auto status = mem.Read(0x8000, nullptr);
  EXPECT_EQ(status, Memory::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, WriteOk) {
  MemoryImpl mem(kTestData);
  auto status = mem.Write(0x8000, 0x01);
  EXPECT_EQ(status, Memory::Status::OK);
}

}  // namespace
