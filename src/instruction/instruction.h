#ifndef INSTRUCTION_INSTRUCTION_H_
#define INSTRUCTION_INSTRUCTION_H_

#include <cstdint>
#include <vector>

#include "src/memory.h"
#include "src/status.h"

class CpuProxy {
 public:
  virtual StatusRegister GetStatusRegister() = 0;
  virtual void SetStatusRegister(StatusRegister status) = 0;
  virtual uint16_t GetPc() = 0;
  virtual void SetPc(uint16_t val) = 0;
  virtual uint8_t GetAcc() = 0;
  virtual void SetAcc(uint8_t val) = 0;
};

class Instruction {
 public:
  Instruction(Memory& mem, CpuProxy& cpu) : mem_(mem), cpu_(cpu) {}
  virtual ~Instruction() {}
  virtual void Execute(uint8_t opcode) = 0;

 protected:
  std::vector<uint8_t> FetchOperands(int num);

  Memory& mem_;
  CpuProxy& cpu_;
};

// No Operation
class NOP : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override {}
};

#endif  // INSTRUCTION_INSTRUCTION_H_
