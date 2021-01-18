#include "src/engine/base/log.h"

#include <iomanip>

namespace {

Log::Level log_level_;
bool found_log_[static_cast<int>(Log::Level::NUM_LEVELS)];

}  // namespace

// static
void Log::Init(Level level) {
  Reset();
  log_level_ = level;
}

// static
void Log::Reset() {
  log_level_ = Log::Level::ERROR;
  for (int i = 0; i < sizeof(found_log_) / sizeof(bool); ++i) {
    found_log_[i] = false;
  }
}

// static
bool Log::ShouldLog(Level level) { return level <= log_level_; }

// static
bool Log::FoundLogWithLevel(Level level) {
  return found_log_[static_cast<int>(level)];
}

// static
std::string Log::Hex(int num) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << num;
  return ss.str();
}

Log::Log(Level level) : level_(level) {}

Log::~Log() {
  found_log_[static_cast<int>(level_)] = true;

  std::string prefix;
  switch (level_) {
    case Level::ERROR:
      prefix = "ERROR: ";
      break;
    case Level::TEST_FAILED:
      prefix = "FAIL:  ";
      break;
    case Level::WARNING:
      prefix = "WARN:  ";
      break;
    case Level::INFO:
      prefix = "INFO:  ";
      break;
    default:
      prefix = "????:  ";
      break;
  }

  // There is no useful information in a test passed log, because gtest already
  // prints the result.
  if (level_ == Level::TEST_PASSED) {
    return;
  }

  std::cout << prefix << stream_.str() << std::endl;
}
