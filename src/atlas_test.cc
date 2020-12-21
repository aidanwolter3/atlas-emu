#include "src/atlas.h"

#include "gtest/gtest.h"
#include "src/cpu.h"

// TODO: Determine how to pass a --headless flag to Atlas so that it does not
// launch a window when running tests.

TEST(AtlasTest, NOP) {
  Atlas atlas("src/roms/nop_test.nes");
  EXPECT_TRUE(atlas.Run());
}

TEST(AtlasTest, PrettyLog) {
  Atlas atlas("src/roms/pretty_log_test.nes");
  EXPECT_TRUE(atlas.Run());
}

TEST(AtlasTest, Branch) {
  Atlas atlas("src/roms/branch_test.nes");
  EXPECT_TRUE(atlas.Run());
}

TEST(AtlasTest, PPU) {
  Atlas atlas("src/roms/ppu_test.nes");
  EXPECT_TRUE(atlas.Run());
}
