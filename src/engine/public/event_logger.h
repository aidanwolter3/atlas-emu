#ifndef ENGINE_PUBLIC_EVENT_LOGGER_H_
#define ENGINE_PUBLIC_EVENT_LOGGER_H_

#include <string>

class EventLogger {
 public:
  virtual ~EventLogger() {}

  enum class EventType {
    kError,
    kInfo,
    kTestFailed,
    kTestPassed,
  };

  struct Event {
    EventType type;
    std::string name;
  };

  virtual void LogEvent(Event event) = 0;
  virtual void PrintLogs() = 0;
};

#endif  // ENGINE_PUBLIC_EVENT_LOGGER_H_
