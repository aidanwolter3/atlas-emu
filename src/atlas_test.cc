#include "src/atlas.h"

#include "gtest/gtest.h"
#include "src/cpu.h"

TEST(AtlasTest, NOP) {
  Atlas atlas("src/roms/nop_test.nes");
  EXPECT_EQ(atlas.RunTimes(4), Cpu::Status::OK);
}

TEST(AtlasTest, PrettyLog) {
  Atlas atlas("src/roms/pretty_log_test.nes");
  EXPECT_EQ(atlas.RunTimes(7), Cpu::Status::OK);
}

TEST(AtlasTest, Branch) {
  Atlas atlas("src/roms/branch_test.nes");
  EXPECT_EQ(atlas.RunTimes(13), Cpu::Status::OK);
}
