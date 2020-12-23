#include "src/platform/posix.h"

#include <time.h>

#include <iostream>
#include <thread>

void PlatformPosix::Sleep(std::chrono::nanoseconds duration) {
  // Reduce the duration by 20ms to account for the time it takes to do the
  // syscall.
  std::this_thread::sleep_for(duration - std::chrono::microseconds{20});
}
