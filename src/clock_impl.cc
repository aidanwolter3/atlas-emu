#include "src/clock_impl.h"

#include <time.h>

#include <iostream>

void PlatformSleepPosix::Sleep(uint64_t time_ns) {
  timespec ts{
      .tv_sec = static_cast<__darwin_time_t>(time_ns / 1000000),
      .tv_nsec = static_cast<long>(time_ns % 1000000),
  };
  // TODO: Capture errors.
  nanosleep(&ts, NULL);
}

ClockImpl::ClockImpl(PlatformSleep& platform_sleep)
    : global_ns_(0), is_running_(false), platform_sleep_(platform_sleep) {}

ClockImpl::~ClockImpl() = default;

void ClockImpl::RegisterTimerObserver(Clock::TimerObserver* observer,
                                      uint64_t period_ns) {
  // TODO: should we pass a reference?

  if (is_running_) {
    std::cout << "Cannot register timer observer when running." << std::endl;
    return;
  }

  if (period_to_observers_map_.count(period_ns) == 0) {
    period_to_observers_map_.insert({period_ns, {}});
  }

  // Add the observer to the list of observers-per-period.
  auto it = period_to_observers_map_.find(period_ns);
  it->second.push_back(observer);
}

void ClockImpl::RunUntilTimer() {
  PrepareRunIfNeeded();

  if (timer_queue_.empty()) {
    return;
  }

  const TimerData& timer_data = timer_queue_.top();

  // Sleep till the next timer should be triggered.
  if (global_ns_ >= timer_data.timer_expiration_ns) {
    // This should only ever happen if the platform is really slow, or if two
    // timers sync up.
    std::cout << "Timer is registered for the past..." << std::endl;
  } else {
    uint64_t sleep_duration = timer_data.timer_expiration_ns - global_ns_;
    platform_sleep_.Sleep(sleep_duration);
    global_ns_ += sleep_duration;
  }

  // Call the observers.
  auto it = period_to_observers_map_.find(timer_data.period_ns);
  if (it == period_to_observers_map_.end()) {
    // TODO: this should never happen.
    std::cout << "No observers for timer..." << std::endl;
    timer_queue_.pop();
    return;
  }
  for (auto* observer : it->second) {
    observer->OnTimerCalled();
  }

  // Re-register the timer for the future.
  timer_queue_.push({
      .period_ns = timer_data.period_ns,
      .timer_expiration_ns =
          timer_data.timer_expiration_ns + timer_data.period_ns,
  });

  // Remove the old timer.
  timer_queue_.pop();
}

void ClockImpl::RunUntilTime(uint64_t time_ns) {
  PrepareRunIfNeeded();

  while (!timer_queue_.empty() &&
         (global_ns_ < time_ns ||
          global_ns_ == timer_queue_.top().timer_expiration_ns)) {
    RunUntilTimer();
  }
}

void ClockImpl::PrepareRunIfNeeded() {
  if (is_running_) {
    return;
  }
  is_running_ = true;

  // Initialize the queue with a timer for every period.
  for (auto it = period_to_observers_map_.begin();
       it != period_to_observers_map_.end(); ++it) {
    timer_queue_.push({
        .period_ns = it->first,
        .timer_expiration_ns = global_ns_ + it->first,
    });
  }
}
