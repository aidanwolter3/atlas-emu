#include "src/atlas.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "src/ui/opengl/renderer.h"
#include "src/ui/opengl/window.h"

namespace {

constexpr int kCpuTicksPerFrame = 1790000 / 60;

std::vector<uint8_t> ReadRomFile(const std::string rom_file) {
  std::vector<uint8_t> rom;

  // Open the ROM file as an input stream.
  std::ifstream rom_stream;
  rom_stream.unsetf(std::ios_base::skipws);
  rom_stream.open(rom_file, std::ios_base::binary);
  if (!rom_stream) {
    std::cout << "Failed to open file: " << rom_file << std::endl;
    return rom;
  }

  // Collect the data from the ROM.
  std::copy(std::istream_iterator<uint8_t>(rom_stream),
            std::istream_iterator<uint8_t>(), std::back_inserter(rom));
  return rom;
}

std::unique_ptr<Window> CreateWindow(bool headless) {
  if (headless) {
    return std::make_unique<FakeWindow>();
  }
  return std::make_unique<OpenGLWindow>();
}

std::unique_ptr<Renderer> CreateRenderer(bool headless) {
  if (headless) {
    return std::make_unique<FakeRenderer>();
  }
  return std::make_unique<OpenGLRenderer>();
}

}  // namespace

Atlas::Atlas(const std::string rom_file, bool headless)
    : window_(CreateWindow(headless)),
      renderer_(CreateRenderer(headless)),
      engine_(*renderer_, ReadRomFile(rom_file)) {}

bool Atlas::Run() {
  auto start_time = std::chrono::steady_clock::now();
  std::chrono::nanoseconds time_slept = std::chrono::nanoseconds{0};
  long long total_ticks = 0;
  while (!window_->IsClosed()) {
    Engine::RunResult run_result = engine_.Run(kCpuTicksPerFrame);
    total_ticks += run_result.num_ticks_ran;

    // Sleep for the expected amount of time to "sync up" with the correct clock
    // speed.
    auto frame_duration = std::chrono::nanoseconds{1000000000 / 60};
    auto now = std::chrono::steady_clock::now();
    auto desired_duration = (frame_duration * total_ticks) / kCpuTicksPerFrame;
    auto sleep_duration = (start_time + desired_duration) - now;
    if (sleep_duration > std::chrono::nanoseconds{0}) {
      time_slept += sleep_duration;
      platform_.Sleep(sleep_duration);
    }

    // Add the clock accuracy and performance to the title.
    std::string title = "Atlas";
    now = std::chrono::steady_clock::now();
    auto time_passed = now - start_time;
    std::chrono::nanoseconds cpu_period{1000000000 / 1790000};
    long long expected_num_ticks = time_passed / cpu_period;
    int clock_speed = (100 * total_ticks) / expected_num_ticks;
    int idle = (100 * time_slept) / time_passed;
    title += " | clock-speed=" + std::to_string(clock_speed) + "%";
    title += " | idle-percentage=" + std::to_string(idle) + "%";
    window_->SetTitle(title);

    // Refresh at the frame-rate.
    window_->Refresh();

    // Exit if needed.
    if (run_result.has_error) return false;
    if (!run_result.can_run) return true;
  }
  return true;
}

void Atlas::Reset() { engine_.Reset(); }
