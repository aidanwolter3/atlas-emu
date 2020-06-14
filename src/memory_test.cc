#include <cstdint>
#include <vector>

#include "gtest/gtest.h"
#include "memory_impl.h"

namespace {

const std::vector<uint8_t> kTestData = {
    // Header
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    0xAB,
    // Code
    0x01,
    0x02,
    0x03,
    0x04,
};

TEST(MemoryTest, ReadOk) {
  uint8_t byte_read;
  MemoryImpl mem(kTestData);
  auto status = mem.Read(Memory::kROMStartAddress, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0x01);
}

TEST(MemoryTest, ReadOutOfBounds) {
  uint8_t byte_read;
  MemoryImpl mem(kTestData);
  auto status = mem.Read(Memory::kROMStartAddress + 4, &byte_read);
  EXPECT_EQ(status, Memory::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, ReadInvalidPointer) {
  MemoryImpl mem(kTestData);
  auto status = mem.Read(Memory::kROMStartAddress, nullptr);
  EXPECT_EQ(status, Memory::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, WriteOk) {
  MemoryImpl mem(kTestData);
  auto status = mem.Write(Memory::kROMStartAddress, 0x01);
  EXPECT_EQ(status, Memory::Status::OK);
}

}  // namespace
