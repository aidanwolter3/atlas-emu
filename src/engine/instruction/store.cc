#include "src/engine/instruction/store.h"

uint8_t STA::Execute(uint8_t opcode, uint16_t operand) { return reg_.acc; }

uint8_t STX::Execute(uint8_t opcode, uint16_t operand) { return reg_.x; }

uint8_t STY::Execute(uint8_t opcode, uint16_t operand) { return reg_.y; }
