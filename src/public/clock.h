#ifndef PUBLIC_CLOCK_H_
#define PUBLIC_CLOCK_H_

#include <chrono>
#include <cstdint>

#define Hz(f) (std::chrono::nanoseconds(1000000000 / f))
#define KHz(f) (Hz(f) / 1000)
#define MHz(f) (KHz(f) / 1000)

class Clock {
 public:
  typedef std::chrono::nanoseconds TimerPeriod;
  typedef std::chrono::time_point<std::chrono::steady_clock> Time;

  virtual ~Clock() {}

  class TimerObserver {
   public:
    virtual ~TimerObserver() {}
    virtual void OnTimerCalled() = 0;
  };

  // Register an observer that receives a callback every |period|. Observers
  // registered for the same |period| will get called on the same clock
  // cycle. All observers must be registered before Run() is called.
  virtual void RegisterTimerObserver(TimerObserver* observer,
                                     TimerPeriod period) = 0;

  virtual void RunUntilTimer() = 0;
  virtual void RunForDuration(std::chrono::nanoseconds duration) = 0;
};

#endif  // PUBLIC_CLOCK_H_
