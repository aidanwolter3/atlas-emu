#include "src/atlas.h"

#include "gtest/gtest.h"

TEST(AtlasTest, Branch) {
  Atlas atlas("src/roms/branch_test.nes", /*headless=*/true);
  EXPECT_TRUE(atlas.Run());
}

TEST(AtlasTest, PPU) {
  Atlas atlas("src/roms/ppu_test.nes", /*headless=*/true);
  EXPECT_TRUE(atlas.Run());
}
