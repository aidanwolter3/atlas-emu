#include <cstdint>
#include <vector>

#include "gtest/gtest.h"
#include "src/memory_impl.h"

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
  data[0x110] = 0xBB;
  data[0x400F] = 0xCC;
  return data;
}

TEST(MemoryTest, ReadBoundaries) {
  uint8_t byte_read;
  Memory::Status status;
  MemoryImpl mem(GenerateLargeTestData(1));

  // first ram byte
  status = mem.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);

  // last ram byte
  status = mem.Read(0x1FFF, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);

  // after last ram byte
  status = mem.Read(0x2000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OUT_OF_BOUNDS);

  // before first prg byte
  status = mem.Read(0x7FFF, &byte_read);
  EXPECT_EQ(status, Memory::Status::OUT_OF_BOUNDS);

  // first prg byte
  status = mem.Read(0x8000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  // last prg byte
  status = mem.Read(0xBFFF, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xCC);
}

TEST(MemoryTest, WriteBoundaries) {
  Memory::Status status;
  MemoryImpl mem(GenerateLargeTestData(1));

  // first ram byte
  status = mem.Write(0x0000, 0xAA);
  EXPECT_EQ(status, Memory::Status::OK);

  // last ram byte
  status = mem.Write(0x1FFF, 0xAA);
  EXPECT_EQ(status, Memory::Status::OK);

  // after last ram byte
  status = mem.Write(0x2000, 0xAA);
  EXPECT_EQ(status, Memory::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, ReadInvalidPointer) {
  MemoryImpl mem(kTestData);
  auto status = mem.Read(0x0000, nullptr);
  EXPECT_EQ(status, Memory::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, ReadFromMirroredRAM) {
  uint8_t byte_read;
  Memory::Status status;
  MemoryImpl mem(kTestData);

  status = mem.Write(0x0000, 0xAA);
  EXPECT_EQ(status, Memory::Status::OK);

  status = mem.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  status = mem.Read(0x0800, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  status = mem.Read(0x1000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  status = mem.Read(0x1800, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);
}

TEST(MemoryTest, WriteToMirroredRAM) {
  uint8_t byte_read;
  Memory::Status status;
  MemoryImpl mem(kTestData);

  status = mem.Write(0x0800, 0xAA);
  EXPECT_EQ(status, Memory::Status::OK);
  status = mem.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  status = mem.Write(0x1000, 0xBB);
  EXPECT_EQ(status, Memory::Status::OK);
  status = mem.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xBB);

  status = mem.Write(0x1800, 0xCC);
  EXPECT_EQ(status, Memory::Status::OK);
  status = mem.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xCC);
}

TEST(MemoryTest, ReadFromMirroredPRG) {
  uint8_t byte_read;
  Memory::Status status;
  MemoryImpl mem(GenerateLargeTestData(1));

  status = mem.Read(0xC000, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  status = mem.Read(0xC100, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xBB);

  status = mem.Read(0xFFFF, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0xCC);
}

TEST(MemoryTest, WriteAndReadRAM) {
  uint8_t byte_read;
  Memory::Status status;
  MemoryImpl mem(kTestData);

  for (uint8_t i = 0; i < 10; ++i) {
    status = mem.Write(i, i);
    EXPECT_EQ(status, Memory::Status::OK);
  }

  for (uint8_t i = 0; i < 10; ++i) {
    status = mem.Read(i, &byte_read);
    EXPECT_EQ(status, Memory::Status::OK);
    EXPECT_EQ(byte_read, i);
  }
}

}  // namespace
