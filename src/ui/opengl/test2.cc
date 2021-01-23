// This is a sanity test that the renderer is working properly.

#include <chrono>
#include <cstdint>
#include <thread>
#include <vector>

#include "src/ui/opengl/renderer2.h"
#include "src/ui/opengl/window.h"

int main() {
  std::vector<uint8_t> frame;
  for (int row = 0; row < 0xF0; ++row) {
    for (int col = 0; col < 0x100; ++col) {
      uint8_t red = 0;
      uint8_t green = 0;
      uint8_t blue = 0;

      if (row < 0xA0) {
        red = (0xA0 - row) * 0xFF / 0xA0;
      }

      if (row > 0x50) {
        green = (row - 0x50) * 0xFF / 0xA0;
      }

      frame.insert(frame.end(), {red, green, blue});
    }
  }

  // Create the window.
  auto window = OpenGLWindow();
  window.SetTitle("Render Test");

  // Create the renderer.
  auto renderer = OpenGLRenderer2();

  // Render until the window is closed.
  while (!window.IsClosed()) {
    renderer.Render(frame);
    window.Refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds{10});

    // Move the first row to the bottom.
    auto row0_end = frame.begin() + (3 * 0x100);
    std::vector<uint8_t> row0(frame.begin(), row0_end);
    frame.erase(frame.begin(), row0_end);
    frame.insert(frame.end(), row0.begin(), row0.end());
  }

  return 0;
}
