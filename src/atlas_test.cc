#include "atlas.h"

#include "cpu.h"
#include "gtest/gtest.h"

TEST(AtlasTest, NOP) {
  Atlas atlas("src/roms/nop_test.nes");
  EXPECT_EQ(atlas.RunTimes(4), Cpu::Status::OK);
}
