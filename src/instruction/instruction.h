#ifndef INSTRUCTION_INSTRUCTION_H_
#define INSTRUCTION_INSTRUCTION_H_

#include <cstdint>
#include <vector>

#include "src/status.h"

class Cpu;

class Instruction {
 public:
  Instruction(Cpu* cpu);
  Instruction(Instruction&& other);
  virtual ~Instruction() {}
  virtual void Execute(uint8_t opcode) = 0;

 protected:
  // Individual instructions cannot access the CPU directly, therefore they use
  // these proxy methods to get/set the memory/registers.
  std::vector<uint8_t> FetchOperands(int num);
  uint8_t ReadMemoryAtOffset(uint16_t offset);
  StatusRegister GetStatusRegister();
  void SetStatusRegister(StatusRegister status);
  void SetAcc(uint16_t val);

 private:
  // We do not want the instructions to be able to directly access the CPU,
  // because then the implementation of each instruction is strongly coupled to
  // the implementation of the CPU.
  Cpu* cpu_;
};

// No Operation
class NOP : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override {}
};

#endif  // INSTRUCTION_INSTRUCTION_H_
