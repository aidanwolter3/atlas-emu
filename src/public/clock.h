#ifndef PUBLIC_CLOCK_H_
#define PUBLIC_CLOCK_H_

#include <cstdint>

class Clock {
 public:
  virtual ~Clock() {}

  class TimerObserver {
   public:
    virtual ~TimerObserver() {}
    virtual void OnTimerCalled() = 0;
  };

  // Register an observer that receives a callback every |period_ns|. Observers
  // registered for the same |period_ns| will get called on the same clock
  // cycle. All observers must be registered before Run() is called.
  virtual void RegisterTimerObserver(TimerObserver* observer,
                                     uint64_t period_ns) = 0;

  virtual void RunUntilTimer() = 0;
  virtual void RunUntilTime(uint64_t time_ns) = 0;
};

#endif  // PUBLIC_CLOCK_H_
