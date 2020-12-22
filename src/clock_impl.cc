#include "src/clock_impl.h"

#include <iostream>

ClockImpl::ClockImpl(Platform& platform)
    : is_running_(false), platform_(platform) {}

ClockImpl::~ClockImpl() = default;

void ClockImpl::RegisterTimerObserver(TimerObserver* observer,
                                      TimerPeriod period) {
  // TODO: should we pass a reference?

  if (is_running_) {
    std::cout << "Cannot register timer observer when running." << std::endl;
    return;
  }

  if (period_to_observers_map_.count(period) == 0) {
    period_to_observers_map_.insert({period, {}});
  }

  // Add the observer to the list of observers-per-period.
  auto it = period_to_observers_map_.find(period);
  it->second.push_back(observer);
}

void ClockImpl::RunUntilTimer() {
  PrepareRunIfNeeded();

  if (timer_queue_.empty()) {
    return;
  }

  const TimerData& timer_data = timer_queue_.top();

  // Sleep till the next timer should be triggered.
  auto now = std::chrono::steady_clock::now();
  if (timer_data.expiration > now) {
    platform_.Sleep(timer_data.expiration - now);
  }

  // Call the observers.
  auto it = period_to_observers_map_.find(timer_data.period);
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
      .expiration = timer_data.expiration + timer_data.period,
      .period = timer_data.period,
  });

  // Remove the old timer.
  timer_queue_.pop();
}

void ClockImpl::RunForDuration(std::chrono::nanoseconds duration) {
  PrepareRunIfNeeded();

  auto stop_time = std::chrono::steady_clock::now() + duration;
  while (!timer_queue_.empty() && timer_queue_.top().expiration < stop_time) {
    RunUntilTimer();
  }
}

void ClockImpl::PrepareRunIfNeeded() {
  // TODO: this probably is not needed.
  if (is_running_) {
    return;
  }
  is_running_ = true;

  // Initialize the queue with a timer for every period.
  auto now = std::chrono::steady_clock::now();
  for (auto it = period_to_observers_map_.begin();
       it != period_to_observers_map_.end(); ++it) {
    timer_queue_.push({
        .expiration = now + it->first,
        .period = it->first,
    });
  }
}
