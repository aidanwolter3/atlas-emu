#include "src/engine/instruction/status.h"

std::optional<uint8_t> CLC::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 2) return std::nullopt;
  reg_.status.reset(Status::kCarry);
  return 0;
}

std::optional<uint8_t> SEC::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 2) return std::nullopt;
  reg_.status.set(Status::kCarry);
  return 0;
}

std::optional<uint8_t> CLI::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 2) return std::nullopt;
  reg_.status.reset(Status::kIntDisable);
  return 0;
}

std::optional<uint8_t> SEI::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 2) return std::nullopt;
  reg_.status.set(Status::kIntDisable);
  return 0;
}

std::optional<uint8_t> CLV::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 2) return std::nullopt;
  reg_.status.reset(Status::kOverflow);
  return 0;
}

std::optional<uint8_t> CLD::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 2) return std::nullopt;
  reg_.status.reset(Status::kBCDMode);
  return 0;
}

std::optional<uint8_t> SED::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 2) return std::nullopt;
  reg_.status.set(Status::kBCDMode);
  return 0;
}
