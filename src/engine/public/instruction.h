#ifndef ENGINE_PUBLIC_INSTRUCTION_H_
#define ENGINE_PUBLIC_INSTRUCTION_H_

#include <cstdint>
#include <vector>

#include "src/engine/public/bus.h"
#include "src/engine/public/registers.h"

#define SET_LOG_NAME(n) \
  std::string GetLogName() override { return n; }

class Instruction {
 public:
  Instruction(Bus& bus, Registers& reg) : bus_(bus), reg_(reg) {}
  virtual ~Instruction() {}
  void Execute(uint8_t opcode);

 protected:
  // Rather than overriding this method, Instructions should use the
  // SET_LOG_NAME macro in their header to provde the Instruction name to log.
  virtual std::string GetLogName();
  // Instructions should override this method to complete their custom
  // execution.
  virtual void ExecuteInternal(uint8_t opcode) {}
  // Grabs the next |num| bytes after the pc, and increments the pc.
  std::vector<uint8_t> FetchOperands(int num);

  // Get the address for all addressing modes.
  uint16_t Immediate();
  uint16_t ZeroPage();
  uint16_t IndexedZeroPage(uint8_t index);
  uint16_t Absolute();
  uint16_t IndirectAbsolute();
  uint16_t IndexedAbsolute(uint8_t index);
  uint16_t IndexedIndirect(uint8_t index);
  uint16_t IndirectIndexed(uint8_t index);

  Bus& bus_;
  Registers& reg_;

  std::vector<std::string> log_elements_;
};

#endif  // ENGINE_PUBLIC_INSTRUCTION_H_
