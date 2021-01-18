#ifndef ENGINE_INSTRUCTION_ADDRESSING_H_
#define ENGINE_INSTRUCTION_ADDRESSING_H_

#include <cstdint>
#include <optional>

#include "src/engine/base/bus.h"
#include "src/engine/base/registers.h"
#include "src/engine/instruction/instruction.h"

class Addressing {
 public:
  Addressing(Bus& bus, Registers& reg);
  bool Execute(Instruction::Config& config, int cycle);

 private:
  struct ExecuteData {
    // The byte(s) immediately following the opcode.
    std::optional<uint16_t> operand;
    // The effective address computed using the |operand|.
    std::optional<uint16_t> address;
    // The byte(s) sent to the instruction for execution.
    // This is often the data found at |address|.
    std::optional<uint16_t> data;
    // How many cycles this instruction should take.
    int cycles;
  };

  void ResetExecuteData();
  bool FetchExecuteData(Instruction::Config& config);
  bool MaybeReadData(Instruction::Config& config);
  bool MaybeWriteData(uint8_t data, Instruction::Config& config);
  std::string ConstructOperandLog(Instruction::Config& config);

  void Implied(Instruction::Operation operation);
  void Immediate(Instruction::Operation operation);
  void ImmediateAddress(Instruction::Operation operation);
  void ZeroPage(Instruction::Operation operation);
  void ZeroPageX(Instruction::Operation operation);
  void ZeroPageY(Instruction::Operation operation);
  void Absolute(Instruction::Operation operation);
  void AbsoluteX(Instruction::Operation operation);
  void AbsoluteY(Instruction::Operation operation);
  void Indirect(Instruction::Operation operation);
  void IndirectX(Instruction::Operation operation);
  void IndirectY(Instruction::Operation operation);

  Bus& bus_;
  Registers& reg_;
  ExecuteData data_;
};

#endif  // ENGINE_INSTRUCTION_ADDRESSING_H_
