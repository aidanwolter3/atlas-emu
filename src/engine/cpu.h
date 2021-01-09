#ifndef ENGINE_CPU_H_
#define ENGINE_CPU_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "src/engine/public/bus.h"
#include "src/engine/public/event_logger.h"
#include "src/engine/public/instruction.h"
#include "src/engine/public/registers.h"

class Cpu {
 public:
  Cpu(EventLogger& event_logger, Bus& bus, Registers& reg);

  void Reset();
  void NMI();
  void Tick();
  void DumpRegisters();

  // Register an Instruction which will be executed for the set of |opcodes|.
  void RegisterInstruction(std::unique_ptr<Instruction2> instruction,
                           std::vector<uint8_t> opcodes);

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

  EventLogger& event_logger_;
  Bus& bus_;
  Registers& reg_;
  std::vector<std::unique_ptr<Instruction2>> instructions_;
  // Map of opcode to Instruction pointer, which points to the Instruction in
  // |instructions_|.
  std::map<uint8_t, Instruction2*> instruction_map_;

  State state_;
  bool nmi_;
  uint8_t opcode_;
  int execute_instruction_ticks_;
  int nmi_ticks_;
};

#endif  // ENGINE_CPU_H_
