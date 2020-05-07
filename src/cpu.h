#ifndef CPU_H_
#define CPU_H_

#include <cstdint>

#include "memory.h"

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
  Stack stack_;
  Memory& mem_;
};

#endif  // CPU_H_
