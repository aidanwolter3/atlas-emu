#ifndef ENGINE_BASE_LOG_H_
#define ENGINE_BASE_LOG_H_

#include <iostream>
#include <sstream>
#include <string>

// Class that is only ever constructed as a temporary variable, which logs a
// message on destruction.
class Log {
 public:
  enum class Level {
    ERROR = 0,
    TEST_FAILED = 1,
    TEST_PASSED = 2,
    WARNING = 3,
    INFO = 4,
    DEBUG = 5,

    NUM_LEVELS = DEBUG + 1,
  };

  // Initialize the logger by setting the maximum log level.
  static void Init(Level level);

  // Resets the logger to default settings.
  static void Reset();

  // Returns whether the log should be created, based on how the |level|
  // compares with the global log level.
  static bool ShouldLog(Level level);

  // Returns whether a log with |level| has been written.
  static bool FoundLogWithLevel(Level level);

  // Helper function for transforming |num| into a hexadecimal string.
  static std::string Hex(int num);

  // This constructor should never be used manually. Instead, use the LOG()
  // macro.
  Log(Level level);
  ~Log();

  std::ostream& stream() { return stream_; }

 private:
  std::ostringstream stream_;
  Level level_;
};

// Helper class to suppress compiler errors when trying to ignore a stream in
// LOG_LEVEL.
class LogVoidify {
 public:
  LogVoidify() = default;
  // This has to be an operator with a precedence lower than << but
  // higher than ?:
  void operator&(std::ostream&) {}
};

#define LOG(level) LOG_LEVEL(Log::Level::level)
#define LOG_LEVEL(level) \
  !(Log::ShouldLog(level)) ? (void)0 : LogVoidify() & (Log(level).stream())

#endif  // ENGINE_BASE_LOG_H_
