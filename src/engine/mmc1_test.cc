#include "src/engine/mmc1.h"

#include <cstdint>
#include <vector>

#include "gtest/gtest.h"

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

class FakePpu : public Ppu {
 public:
  void Tick() override {}
};

TEST(MMC1Test, ReadBoundaries) {
  uint8_t byte_read;
  Peripheral::Status status;
  FakePpu ppu;
  MMC1Impl mmc1(ppu, GenerateLargeTestData(1));

  // first prg byte
  status = mmc1.Read(0x0000, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  // last prg byte
  status = mmc1.Read(0x3FFF, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xCC);
}

TEST(MMC1Test, ReadInvalidPointer) {
  FakePpu ppu;
  MMC1Impl mmc1(ppu, kTestData);
  auto status = mmc1.Read(0x0000, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OUT_OF_BOUNDS);
}

TEST(MMC1Test, ReadFromMirroredPRG) {
  uint8_t byte_read;
  Peripheral::Status status;
  FakePpu ppu;
  MMC1Impl mmc1(ppu, GenerateLargeTestData(1));

  status = mmc1.Read(0x4000, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xAA);

  status = mmc1.Read(0x4100, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xBB);

  status = mmc1.Read(0x7FFF, &byte_read);
  EXPECT_EQ(status, Peripheral::Status::OK);
  EXPECT_EQ(byte_read, 0xCC);
}

}  // namespace
