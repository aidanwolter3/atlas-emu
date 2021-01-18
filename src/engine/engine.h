#ifndef ENGINE_ENGINE_H_
#define ENGINE_ENGINE_H_

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "src/engine/base/registers.h"
#include "src/engine/bus_impl.h"
#include "src/engine/cpu.h"
#include "src/engine/instruction/instruction.h"
#include "src/engine/joystick.h"
#include "src/engine/memory.h"
#include "src/engine/mmc1.h"
#include "src/engine/oamdma.h"
#include "src/engine/ppu.h"
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
  using Mode = Instruction::Mode;
  using Operation = Instruction::Operation;

  void DumpState();
  void RegisterInstructions();
  // Register |opcode| for the templated instruction with address |mode|.
  // Constructs the addressing mode if it does not already exist.
  // Constructs the instruction if it does not alredy exist.
  template <class INS>
  void RegisterInstruction(uint8_t opcode, Mode mode = Mode::kImplied,
                           Operation operation = Operation::kNone);

  // Map of opcodes to instructions.
  std::unordered_map<uint8_t, std::unique_ptr<Instruction>> instructions_;

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
