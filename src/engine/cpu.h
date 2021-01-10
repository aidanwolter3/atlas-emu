#ifndef ENGINE_CPU_H_
#define ENGINE_CPU_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "src/engine/instruction/addressing_mode.h"
#include "src/engine/instruction/instruction.h"
#include "src/engine/public/bus.h"
#include "src/engine/public/event_logger.h"
#include "src/engine/public/registers.h"

struct InstructionConfig {
  AddressingMode* mode;
  Instruction2* instruction;
};

class Cpu {
 public:
  Cpu(EventLogger& event_logger, Bus& bus, Registers& reg);

  void Reset();
  void NMI();
  void Tick();
  void DumpRegisters();

  // Register an Instruction which will be executed for given |opcode|.
  // If the |config| specifies a nullptr mode, then zero operands are fetched.
  // If the |config| specifies a nullptr instruction, then no instruction is
  // executed.
  void RegisterInstruction(uint8_t opcode, InstructionConfig config);

 private:
  enum class State {
    kFetchOpcode,
    kFetchOperand,
    kExecuteInstruction,
    kNMI,
  };

  void FetchOpcode();
  bool FetchOperand();
  bool ExecuteInstruction();
  bool PerformNMI();
  uint16_t ReadAddressFromVectorTable(uint16_t address);

  EventLogger& event_logger_;
  Bus& bus_;
  Registers& reg_;

  // Map of opcode to InstructionConfig
  std::map<uint8_t, InstructionConfig> instructions_;

  State state_;
  bool nmi_;
  uint8_t opcode_;
  uint16_t operand_;
  int instruction_ticks_;
  int nmi_ticks_;
};

#endif  // ENGINE_CPU_H_
