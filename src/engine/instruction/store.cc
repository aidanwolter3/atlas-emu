#include "src/engine/instruction/store.h"

Instruction::ExecuteResult STA::Execute(uint16_t operand) {
  return {.data = reg_.acc};
}

Instruction::ExecuteResult STX::Execute(uint16_t operand) {
  return {.data = reg_.x};
}

Instruction::ExecuteResult STY::Execute(uint16_t operand) {
  return {.data = reg_.y};
}
