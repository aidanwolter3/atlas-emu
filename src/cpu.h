#ifndef CPU_H_
#define CPU_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "src/memory.h"
#include "src/public/instruction.h"
#include "src/public/registers.h"

class Cpu {
 public:
  Cpu(Memory& mem, Registers& reg);
  ~Cpu();

  // TODO: Consider prefixing each instruction with the stage (FETCH, DECODE).
  // TODO: Override the operator<< to print the status name.
  enum class Status : uint8_t {
    OK,
    SEGFAULT,
    UNKNOWN_INSTRUCTION,
  };

  // Register an Instruction which will be executed for the set of |opcodes|.
  void RegisterInstruction(std::unique_ptr<Instruction> instruction,
                           std::vector<uint8_t> opcodes);

  // Run a single clock cycle.
  Cpu::Status Run();

 private:
  // Fetch the |opcode| at |location| in |mem_|.
  Cpu::Status Fetch(uint16_t location, uint8_t* opcode);

  Memory& mem_;
  Registers& reg_;
  std::vector<std::unique_ptr<Instruction>> instructions_;
  // Map of opcode to Instruction pointer, which points to the Instruction in
  // |instructions_|..
  std::map<uint8_t, Instruction*> instruction_map_;
};

#endif  // CPU_H_
