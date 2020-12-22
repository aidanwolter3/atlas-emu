#ifndef PLATFORM_POSIX_H_
#define PLATFORM_POSIX_H_

#include "src/platform/platform.h"

class PlatformPosix : public Platform {
  void Sleep(std::chrono::nanoseconds duration) override;
};

#endif  // PLATFORM_POSIX_H_
