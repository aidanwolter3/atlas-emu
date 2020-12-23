#include "src/event_logger_impl.h"

#include <iostream>

namespace {

std::string EventTypeToString(EventLogger::EventType type) {
  switch (type) {
    case EventLogger::EventType::kError:
      return "ERROR";
    case EventLogger::EventType::kInfo:
      return "INFO";
    case EventLogger::EventType::kTestFailed:
      return "TEST_FAILED";
    case EventLogger::EventType::kTestPassed:
      return "TEST_PASSED";
  }
  return "UNKNOWN";
}

}  // namespace

EventLoggerImpl::EventLoggerImpl() = default;
EventLoggerImpl::~EventLoggerImpl() = default;

void EventLoggerImpl::LogEvent(Event event) {
  if (!first_error_ && event.type == EventType::kError) {
    first_error_ = event;
  } else if (!first_test_result_ && (event.type == EventType::kTestFailed ||
                                     event.type == EventType::kTestPassed)) {
    first_test_result_ = event;
  }
  events_.push_back(event);
}

void EventLoggerImpl::Reset() {
  first_error_ = std::nullopt;
  first_test_result_ = std::nullopt;
  events_.clear();
}

std::optional<EventLogger::Event> EventLoggerImpl::GetError() {
  return first_error_;
}

std::optional<EventLogger::Event> EventLoggerImpl::GetTestResult() {
  return first_test_result_;
}

void EventLoggerImpl::PrintLogs() {
  for (auto& event : events_) {
    std::cout << EventTypeToString(event.type) << ": " << event.name
              << std::endl;
  }
}
