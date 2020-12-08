#ifndef CPU_H_
#define CPU_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "src/public/bus.h"
#include "src/public/clock.h"
#include "src/public/event_logger.h"
#include "src/public/instruction.h"
#include "src/public/registers.h"

class Cpu : public Clock::TimerObserver {
 public:
  Cpu(EventLogger& event_logger, Clock& clock, Bus& bus, Registers& reg);
  ~Cpu() override;

  // Register an Instruction which will be executed for the set of |opcodes|.
  void RegisterInstruction(std::unique_ptr<Instruction> instruction,
                           std::vector<uint8_t> opcodes);

 private:
  // Clock::TimerObserver implementation:
  void OnTimerCalled() override;

  EventLogger& event_logger_;
  Bus& bus_;
  Registers& reg_;
  std::vector<std::unique_ptr<Instruction>> instructions_;
  // Map of opcode to Instruction pointer, which points to the Instruction in
  // |instructions_|..
  std::map<uint8_t, Instruction*> instruction_map_;
};

#endif  // CPU_H_
