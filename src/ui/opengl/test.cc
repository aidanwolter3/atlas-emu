// This is a sanity test that the renderer is working properly.
//
// It uses the Window/Renderer abstractions to render each block with a
// different palette, and an arrow in the top-left corner pointing down-right.
// It also renders a "one" sprite in the center of the top-left block.

#include <cstdint>
#include <vector>

#include "src/ui/color.h"
#include "src/ui/opengl/renderer.h"
#include "src/ui/opengl/window.h"

constexpr int kTileSize = 64;
constexpr int kRowSize = 32 * kTileSize;

void InsertTileAtLocation(std::vector<uint8_t>& nametable,
                          std::vector<uint8_t>& tile, int x, int y) {
  int x_offset = x * 8;
  int y_offset = y * kRowSize;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      nametable[(y_offset + x_offset) + (i * 32 * 8) + j] = tile[(i * 8) + j];
    }
  }
}

int main() {
  std::vector<uint8_t> frame_palette = {
      0, 33, 0, 0, 48, 21, 0, 0, 49, 41, 0, 0, 18, 49, 0, 0,
      0, 33, 0, 0, 48, 21, 0, 0, 49, 41, 0, 0, 18, 49, 0, 0,
  };
  std::vector<uint8_t> attribute_table;
  uint8_t attributes[] = {
      0b00000000, 0b01010101, 0b10101010, 0b11111111, 0b00000000, 0b01010101,
      0b10101010, 0b01010101, 0b10101010, 0b11111111, 0b00000000, 0b01010101,
      0b10101010, 0b11111111, 0b01010101, 0b00000000,
  };
  for (int i = 0; i < 0x40; ++i) {
    attribute_table.push_back(attributes[i % 16]);
  }

  // Assemble the nametable.
  std::vector<uint8_t> nametable(30 * kRowSize, 0);

  // Insert an arrow in each block pointing down-to-the-right.
  std::vector<uint8_t> arrow = {
      0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1,
      1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
  };
  for (int i = 0; i < 30; i += 4) {
    for (int j = 0; j < 32; j += 4) {
      InsertTileAtLocation(nametable, arrow, j, i);
    }
  }

  // Assemble the sprites.
  std::vector<uint8_t> sprite_tiles(64 * 64, 0);
  std::vector<Sprite> sprites;
  std::vector<uint8_t> one = {
      0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0,
      0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
  };
  for (int i = 0; i < one.size(); ++i) {
    sprite_tiles[i] = one[i];
  }
  Sprite null_sprite{.x = 0xFF, .y = 0xFF, .tile_num = 0x01, .palette = 0x00};
  for (int i = 0; i < 64; ++i) {
    sprites.push_back(null_sprite);
  }
  Sprite one_sprite{
      .x = 0x0C,
      .y = 0x0C,
      .tile_num = 0x00,
      .palette = 0x00,
  };
  sprites[0] = one_sprite;

  // Create the window.
  auto window = OpenGLWindow(/*headless=*/false);
  window.SetTitle("Render Test");

  // Create the renderer.
  auto renderer = OpenGLRenderer();
  renderer.SetPalette(kColorPalette);
  renderer.SetFramePalette(frame_palette);
  renderer.SetAttributeTable(0, attribute_table);
  renderer.SetNametable(0, nametable);
  renderer.SetSpriteTiles(sprite_tiles);
  renderer.SetSprites(sprites);

  // Render until the window is closed.
  while (!window.IsClosed()) {
    window.Refresh();
    renderer.Render();
  }

  return 0;
}
