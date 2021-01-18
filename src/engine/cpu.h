#ifndef ENGINE_CPU_H_
#define ENGINE_CPU_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "src/engine/base/bus.h"
#include "src/engine/base/registers.h"
#include "src/engine/instruction/addressing.h"

class Cpu {
 public:
  Cpu(Bus& bus, Registers& reg);

  void Reset();
  void NMI();
  void Tick();
  void DumpRegisters();

  // Register an Instruction using |config|.
  // If the |config| specifies a nullptr mode, then zero operands are fetched.
  // If the |config| specifies a nullptr instruction, then no instruction is
  // executed.
  void RegisterInstruction(Instruction::Config config);

 private:
  enum class State {
    kFetchOpcode,
    kExecuteInstruction,
    kNMI,
  };

  void FetchOpcode();
  bool ExecuteInstruction();
  bool PerformNMI();
  uint16_t ReadAddressFromVectorTable(uint16_t address);

  Bus& bus_;
  Registers& reg_;
  Addressing addressing_;

  // Map of opcode to Instruction::Config.
  std::map<uint8_t, Instruction::Config> instructions_;

  State state_;
  bool nmi_;
  uint8_t opcode_;
  int instruction_ticks_;
  int nmi_ticks_;
};

#endif  // ENGINE_CPU_H_
