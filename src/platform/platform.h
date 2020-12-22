#ifndef PUBLIC_PLATFORM_H_
#define PUBLIC_PLATFORM_H_

#include <chrono>
#include <cstdint>

class Platform {
 public:
  virtual void Sleep(std::chrono::nanoseconds duration) = 0;
};

#endif  // PUBLIC_PLATFORM_H_
