#include "memory.h"

#include <cstdint>
#include <vector>

#include "gtest/gtest.h"

namespace {

const std::vector<uint8_t> kTestData = {
  0x01, 0x02, 0x03, 0x04,
};

TEST(MemoryTest, ReadOk) {
  uint8_t byte_read;
  Memory mem(kTestData);
  auto status = mem.Read(0, &byte_read);
  EXPECT_EQ(status, Memory::Status::OK);
  EXPECT_EQ(byte_read, 0x01);
}

TEST(MemoryTest, ReadOutOfBounds) {
  uint8_t byte_read;
  Memory mem(kTestData);
  auto status = mem.Read(4, &byte_read);
  EXPECT_EQ(status, Memory::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, ReadInvalidPointer) {
  Memory mem(kTestData);
  auto status = mem.Read(0, nullptr);
  EXPECT_EQ(status, Memory::Status::OUT_OF_BOUNDS);
}

TEST(MemoryTest, WriteOk) {
  Memory mem(kTestData);
  auto status = mem.Write(0, 0x01);
  EXPECT_EQ(status, Memory::Status::OK);
}

}  // namespace
