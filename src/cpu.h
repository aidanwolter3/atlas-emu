#ifndef CPU_H_
#define CPU_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "memory.h"

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
  class Instruction;

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

  // Only for testing.
  uint16_t GetPc() { return pc_; }
  StatusRegister GetStatusRegister() { return status_; }

 private:
  // Construct and register an instruction with the classname INS that will get
  // executed when the |opcode| is fetched.
  template <class INS>
  void RegisterInstruction(std::vector<uint8_t> opcodes);
  template <class INS>
  void RegisterInstruction(uint8_t opcode);

  // Fetch the |opcode| at |location| in |mem_|.
  Cpu::Status Fetch(uint16_t location, uint8_t* opcode);

  // Registers
  uint16_t pc_;
  StatusRegister status_;
  uint16_t acc_;

  std::map<uint8_t, std::shared_ptr<Instruction>> instructions_;
  Memory& mem_;
};

#endif  // CPU_H_
