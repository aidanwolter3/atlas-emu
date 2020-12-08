#ifndef EVENT_LOGGER_IMPL_H_
#define EVENT_LOGGER_IMPL_H_

#include <optional>
#include <vector>

#include "src/public/event_logger.h"

class EventLoggerImpl : public EventLogger {
 public:
  EventLoggerImpl();
  ~EventLoggerImpl() override;

  void LogEvent(Event event) override;
  void PrintLogs() override;

  std::optional<Event> GetError();
  std::optional<Event> GetTestResult();

 private:
  std::vector<Event> events_;
  std::optional<Event> first_error_;
  std::optional<Event> first_test_result_;
};

#endif  // EVENT_LOGGER_IMPL_H_
