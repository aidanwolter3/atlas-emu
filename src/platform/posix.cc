#include "src/platform/posix.h"

#include <time.h>

#include <iostream>
#include <thread>

void PlatformPosix::Sleep(std::chrono::nanoseconds duration) {
  // Reduce the duration by 20ms to account for the time it takes to do the
  // syscall.
  auto real_duration = duration - std::chrono::microseconds{20};
  if (duration > std::chrono::nanoseconds{0}) {
    std::this_thread::sleep_for(real_duration);
  }
}
