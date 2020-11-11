#ifndef CPU_H_
#define CPU_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "src/instruction/instruction.h"
#include "src/memory.h"
#include "src/registers.h"

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

  // Run a single clock cycle.
  Cpu::Status Run();

 private:
  // Construct and register an instruction with the classname INS that will get
  // executed when the |opcode| is fetched.
  template <class INS>
  void RegisterInstruction(std::vector<uint8_t> opcodes);
  template <class INS>
  void RegisterInstruction(uint8_t opcode);

  // Fetch the |opcode| at |location| in |mem_|.
  Cpu::Status Fetch(uint16_t location, uint8_t* opcode);

  Memory& mem_;
  Registers& reg_;
  std::map<uint8_t, std::shared_ptr<Instruction>> instructions_;
};

#endif  // CPU_H_
