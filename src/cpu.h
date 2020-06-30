#ifndef CPU_H_
#define CPU_H_

#include <cstdint>

#include "memory.h"

class Instruction {};
class NOP : public Instruction {};
struct StatusRegister {
  bool carry = false;
  bool zero = false;
  bool int_disable = false;
  bool bcd_mode = false;
  bool brk = false;
  bool overflow = false;
  bool sign = false;
};

class Cpu {
 public:
  Cpu(Memory& mem);
  ~Cpu();

  // TODO: Consider prefixing each instruction with the stage (FETCH, DECODE).
  // TODO: Override the operator<< to print the status name.
  enum class Status : uint8_t {
    OK,
    SEGFAULT,
    UNKNOWN_INSTRUCTION,
  };

  // Run a single clock cycle.
  Cpu::Status Run();

  uint16_t GetPc() { return pc_; }
  StatusRegister& GetStatusRegister() { return status_; }

 private:
  Cpu::Status Fetch(uint16_t location, uint8_t* hex_instruction);
  Cpu::Status Decode(uint8_t hex_instruction, Instruction* instruction);
  // Registers
  uint16_t pc_;
  StatusRegister status_;

  Memory& mem_;
};

#endif  // CPU_H_
