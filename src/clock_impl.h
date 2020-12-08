#ifndef CLOCK_IMPL_H_
#define CLOCK_IMPL_H_

#include <cstdint>
#include <functional>
#include <map>
#include <queue>

#include "src/public/clock.h"

class PlatformSleep {
 public:
  virtual void Sleep(uint64_t time_ns) = 0;
};

class PlatformSleepPosix : public PlatformSleep {
  void Sleep(uint64_t time_ns) override;
};

class ClockImpl : public Clock {
 public:
  explicit ClockImpl(PlatformSleep& platform_sleep);
  ~ClockImpl() override;

  // Clock implementation:
  void RegisterTimerObserver(Clock::TimerObserver* observer,
                             uint64_t period_ns) override;
  void RunUntilTimer() override;
  void RunUntilTime(uint64_t time_ns) override;

 private:
  void PrepareRunIfNeeded();

  uint64_t global_ns_;
  bool is_running_;
  PlatformSleep& platform_sleep_;

  struct TimerData {
    typedef uint64_t value_type;
    uint64_t timer_expiration_ns;
    uint64_t period_ns;

    friend bool operator>(const TimerData& data1, const TimerData& data2) {
      return data1.timer_expiration_ns > data2.timer_expiration_ns;
    }
  };

  std::map<uint64_t, std::vector<Clock::TimerObserver*>>
      period_to_observers_map_;
  std::priority_queue<TimerData, std::vector<TimerData>,
                      std::greater<TimerData>>
      timer_queue_;
};

#endif  // CLOCK_IMPL_H_
