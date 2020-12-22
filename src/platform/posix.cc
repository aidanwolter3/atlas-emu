#include "src/platform/posix.h"

#include <thread>

void PlatformPosix::Sleep(std::chrono::nanoseconds duration) {
  std::this_thread::sleep_for(duration);
}
