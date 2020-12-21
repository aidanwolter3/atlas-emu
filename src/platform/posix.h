#ifndef PLATFORM_POSIX_H_
#define PLATFORM_POSIX_H_

#include "src/platform/platform.h"

class PlatformPosix : public Platform {
  void SleepNanoseconds(uint64_t time_ns) override;
};

#endif  // PLATFORM_POSIX_H_
