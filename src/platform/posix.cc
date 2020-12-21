#include "src/platform/posix.h"

#include <time.h>

void PlatformPosix::SleepNanoseconds(uint64_t time_ns) {
  timespec ts{
      .tv_sec = static_cast<time_t>(time_ns / 1000000),
      .tv_nsec = static_cast<long>(time_ns % 1000000),
  };
  // TODO: Capture errors.
  nanosleep(&ts, NULL);
}
