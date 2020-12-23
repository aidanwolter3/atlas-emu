#ifndef CLOCK_IMPL_H_
#define CLOCK_IMPL_H_

#include <chrono>
#include <cstdint>
#include <functional>
#include <map>
#include <queue>
#include <vector>

#include "src/platform/platform.h"
#include "src/public/clock.h"

class ClockImpl : public Clock {
 public:
  explicit ClockImpl(Platform& platform);
  ~ClockImpl() override;

  // Clock implementation:
  void RegisterTimerObserver(TimerObserver* observer,
                             TimerPeriod period) override;
  void RunUntilTimer() override;
  void RunForDuration(std::chrono::nanoseconds duration) override;

 private:
  void CompleteTopTimer();
  void PrepareRunIfNeeded();

  bool is_running_;
  Platform& platform_;

  struct TimerData {
    typedef uint64_t value_type;
    Time expiration;
    TimerPeriod period;

    friend bool operator>(const TimerData& data1, const TimerData& data2) {
      return data1.expiration > data2.expiration;
    }
  };

  std::map<TimerPeriod, std::vector<TimerObserver*>> period_to_observers_map_;
  std::priority_queue<TimerData, std::vector<TimerData>,
                      std::greater<TimerData>>
      timer_queue_;
};

#endif  // CLOCK_IMPL_H_
