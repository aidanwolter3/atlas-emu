#ifndef INSTRUCTION_INSTRUCTION_H_
#define INSTRUCTION_INSTRUCTION_H_

#include <cstdint>
#include <vector>

#include "src/status.h"

class CpuProxy {
 public:
  virtual std::vector<uint8_t> FetchOperands(int num) = 0;
  virtual uint8_t ReadMemoryAtOffset(uint16_t offset) = 0;
  virtual void WriteMemoryAtOffset(uint16_t offset, uint8_t data) = 0;
  virtual StatusRegister GetStatusRegister() = 0;
  virtual void SetStatusRegister(StatusRegister status) = 0;
  // TODO: The accumulator is only 8-bit.
  virtual uint16_t GetAcc() = 0;
  virtual void SetAcc(uint16_t val) = 0;
};

class Instruction {
 public:
  Instruction(CpuProxy& cpu) : cpu_(cpu) {}
  virtual ~Instruction() {}
  virtual void Execute(uint8_t opcode) = 0;

 protected:
  CpuProxy& cpu_;
};

// No Operation
class NOP : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override {}
};

#endif  // INSTRUCTION_INSTRUCTION_H_
