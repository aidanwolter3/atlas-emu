#include "src/memory.h"

#include <cstdint>

#include "gtest/gtest.h"

namespace {

TEST(MemoryTest, ReadBoundaries) {
  uint8_t byte_read;
  Peripheral::Status status;
  MemoryImpl mem;

  // first ram byte
  status = mem.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);

  // last ram byte
  status = mem.Read(0x1FFF, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);

  // after last ram byte
  status = mem.Read(0x2000, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, WriteBoundaries) {
  Peripheral::Status status;
  MemoryImpl mem;

  // first ram byte
  status = mem.Write(0x0000, 0xAA);
  EXPECT_EQ(status, Peripheral::Status::OK);

  // last ram byte
  status = mem.Write(0x1FFF, 0xAA);
  EXPECT_EQ(status, Peripheral::Status::OK);

  // after last ram byte
  status = mem.Write(0x2000, 0xAA);
  EXPECT_EQ(status, Peripheral::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, ReadInvalidPointer) {
  MemoryImpl mem;
  auto status = mem.Read(0x0000, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, ReadFromMirroredRAM) {
  uint8_t byte_read;
  Peripheral::Status status;
  MemoryImpl mem;

  status = mem.Write(0x0000, 0xAA);
  EXPECT_EQ(status, Peripheral::Status::OK);

  status = mem.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  status = mem.Read(0x0800, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  status = mem.Read(0x1000, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  status = mem.Read(0x1800, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);
}

TEST(MemoryTest, WriteToMirroredRAM) {
  uint8_t byte_read;
  Peripheral::Status status;
  MemoryImpl mem;

  status = mem.Write(0x0800, 0xAA);
  EXPECT_EQ(status, Peripheral::Status::OK);
  status = mem.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  status = mem.Write(0x1000, 0xBB);
  EXPECT_EQ(status, Peripheral::Status::OK);
  status = mem.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xBB);

  status = mem.Write(0x1800, 0xCC);
  EXPECT_EQ(status, Peripheral::Status::OK);
  status = mem.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xCC);
}

TEST(MemoryTest, WriteAndReadRAM) {
  uint8_t byte_read;
  Peripheral::Status status;
  MemoryImpl mem;

  for (uint8_t i = 0; i < 10; ++i) {
    status = mem.Write(i, i);
    EXPECT_EQ(status, Peripheral::Status::OK);
  }

  for (uint8_t i = 0; i < 10; ++i) {
    status = mem.Read(i, &byte_read);
    EXPECT_EQ(status, Peripheral::Status::OK);
    EXPECT_EQ(byte_read, i);
  }
}

}  // namespace
