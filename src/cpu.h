#ifndef CPU_H_
#define CPU_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "src/public/bus.h"
#include "src/public/event_logger.h"
#include "src/public/instruction.h"
#include "src/public/registers.h"

class Cpu {
 public:
  Cpu(EventLogger& event_logger, Bus& bus, Registers& reg);

  void Reset();
  void NMI();
  void Tick();

  // Register an Instruction which will be executed for the set of |opcodes|.
  void RegisterInstruction(std::unique_ptr<Instruction> instruction,
                           std::vector<uint8_t> opcodes);

 private:
  uint16_t ReadAddressFromVectorTable(uint16_t address);

  EventLogger& event_logger_;
  Bus& bus_;
  Registers& reg_;
  std::vector<std::unique_ptr<Instruction>> instructions_;
  // Map of opcode to Instruction pointer, which points to the Instruction in
  // |instructions_|..
  std::map<uint8_t, Instruction*> instruction_map_;
};

#endif  // CPU_H_
