#ifndef ENGINE_ENGINE_H_
#define ENGINE_ENGINE_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "src/engine/bus_impl.h"
#include "src/engine/cpu.h"
#include "src/engine/event_logger_impl.h"
#include "src/engine/joystick.h"
#include "src/engine/memory.h"
#include "src/engine/mmc1.h"
#include "src/engine/oamdma.h"
#include "src/engine/ppu.h"
#include "src/engine/public/registers.h"
#include "src/input/input.h"
#include "src/ui/renderer.h"

class Engine {
 public:
  Engine(Input& input, Renderer& renderer, std::vector<uint8_t> rom);

  // Result of Run(), which returns the number of ticks was actually run, if the
  // engine can continue running, and whether an error has occurred.
  struct RunResult {
    int num_ticks_ran;
    bool can_run;
    bool has_error;
  };

  // Run for |num_ticks| ticks, then render.
  RunResult Run(int num_ticks);

  // Reset the CPU, so that it can be run again. Useful for benchmarking, as we
  // run multiple times.
  void Reset();

 private:
  void DumpState();
  void RegisterInstructions();
  // Construct and register an instruction with the classname INS that will get
  // executed when |opcode| is fetched.
  template <class INS>
  void RegisterInstruction(uint8_t opcode);
  // Construct and register an instruction with the classname INS that will get
  // executed when one of the |opcodes| are fetched.
  template <class INS>
  void RegisterInstruction(std::vector<uint8_t> opcodes);

  EventLoggerImpl event_logger_;
  BusImpl bus_;
  Registers reg_;
  OAMDMA oamdma_;
  Joystick joystick_;
  std::unique_ptr<MemoryImpl> mem_;
  std::unique_ptr<MemoryImpl> mmc1_mem_;
  std::unique_ptr<MMC1Impl> mmc1_;
  std::unique_ptr<Cpu> cpu_;
  std::unique_ptr<PpuImpl> ppu_;
};

#endif  // ENGINE_ENGINE_H_
