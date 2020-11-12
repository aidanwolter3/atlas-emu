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
  // Grabs the next |num| bytes after the pc, and increments the pc.
  std::vector<uint8_t> FetchOperands(int num);

  // Get the address for all addressing modes.
  uint16_t Immediate();
  uint16_t ZeroPage();
  uint16_t IndexedZeroPage(uint8_t index);
  uint16_t Absolute();
  uint16_t IndexedAbsolute(uint8_t index);
  uint16_t IndexedIndirect(uint8_t index);
  uint16_t IndirectIndexed(uint8_t index);

  Memory& mem_;
  Registers& reg_;
};

#endif  // PUBLIC_INSTRUCTION_H_
