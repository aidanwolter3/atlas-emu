#ifndef CPU_H_
#define CPU_H_

#include <cstdint>

#include "memory.h"

class Instruction {};

class Cpu {
 public:
  Cpu(Memory& mem);
  ~Cpu();

  enum class Status : uint8_t {
    OK,
    SEGFAULT,
  };

  struct Stack {
    uint16_t pc;
  };

  // Run a single clock cycle.
  Cpu::Status Run();

  uint16_t GetPc() { return stack_.pc; }

 private:
  Cpu::Status Fetch(uint16_t location, uint8_t* hex_instruction);
  Cpu::Status Decode(uint8_t hex_instruction, Instruction* instruction);

  Stack stack_;
  Memory& mem_;
};

#endif  // CPU_H_
