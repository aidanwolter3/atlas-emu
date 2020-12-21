#ifndef PUBLIC_PLATFORM_H_
#define PUBLIC_PLATFORM_H_

#include <cstdint>

class Platform {
 public:
  virtual void SleepNanoseconds(uint64_t time_ns) = 0;
};

#endif  // PUBLIC_PLATFORM_H_
