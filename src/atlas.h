#ifndef ATLAS_H_
#define ATLAS_H_

#include <memory>
#include <string>

#include "src/bus_impl.h"
#include "src/clock_impl.h"
#include "src/cpu.h"
#include "src/event_logger_impl.h"
#include "src/memory.h"
#include "src/mmc1.h"
#include "src/ppu.h"
#include "src/public/registers.h"
#include "src/ui/window.h"

class Atlas {
 public:
  Atlas(const std::string rom_file, bool headless = false);
  ~Atlas();

  // Run the CPU until an error occurs, and returns true for success, or false
  // for error.
  bool Run();

 private:
  // Construct and register an instruction with the classname INS that will get
  // executed when |opcode| is fetched.
  template <class INS>
  void RegisterInstruction(uint8_t opcode);
  // Construct and register an instruction with the classname INS that will get
  // executed when one of the |opcodes| are fetched.
  template <class INS>
  void RegisterInstruction(std::vector<uint8_t> opcodes);

  EventLoggerImpl event_logger_;
  PlatformSleepPosix platform_sleep_;
  ClockImpl clock_;
  BusImpl bus_;
  Registers reg_;
  std::unique_ptr<Window> window_;
  std::unique_ptr<MemoryImpl> mem_;
  std::unique_ptr<MemoryImpl> mmc1_mem_;
  std::unique_ptr<MMC1Impl> mmc1_;
  std::unique_ptr<Cpu> cpu_;
  std::unique_ptr<Ppu> ppu_;
};

#endif  // ATLAS_H_
