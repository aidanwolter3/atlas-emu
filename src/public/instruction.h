#ifndef PUBLIC_INSTRUCTION_H_
#define PUBLIC_INSTRUCTION_H_

#include <cstdint>
#include <vector>

#include "src/public/memory.h"
#include "src/public/registers.h"

class Instruction {
 public:
  Instruction(Memory& mem, Registers& reg) : mem_(mem), reg_(reg) {}
  virtual ~Instruction() {}
  virtual void Execute(uint8_t opcode) = 0;

 protected:
  std::vector<uint8_t> FetchOperands(int num);

  Memory& mem_;
  Registers& reg_;
};

#endif  // PUBLIC_INSTRUCTION_H_
