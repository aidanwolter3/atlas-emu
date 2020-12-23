#include "src/instruction/branch.h"

void BPL::ExecuteInternal(uint8_t opcode) {
  uint8_t offset = Immediate();
  if (!reg_.status.test(Status::kSign)) {
    reg_.pc += static_cast<int8_t>(offset);
    // log_elements_.push_back("-> true");
  }
}

void BMI::ExecuteInternal(uint8_t opcode) {
  uint8_t offset = Immediate();
  if (reg_.status.test(Status::kSign)) {
    reg_.pc += static_cast<int8_t>(offset);
    // log_elements_.push_back("-> true");
  }
}

void BVC::ExecuteInternal(uint8_t opcode) {
  uint8_t offset = Immediate();
  if (!reg_.status.test(Status::kOverflow)) {
    reg_.pc += static_cast<int8_t>(offset);
    // log_elements_.push_back("-> true");
  }
}

void BVS::ExecuteInternal(uint8_t opcode) {
  uint8_t offset = Immediate();
  if (reg_.status.test(Status::kOverflow)) {
    reg_.pc += static_cast<int8_t>(offset);
    // log_elements_.push_back("-> true");
  }
}

void BCC::ExecuteInternal(uint8_t opcode) {
  uint8_t offset = Immediate();
  if (!reg_.status.test(Status::kCarry)) {
    reg_.pc += static_cast<int8_t>(offset);
    // log_elements_.push_back("-> true");
  }
}

void BCS::ExecuteInternal(uint8_t opcode) {
  uint8_t offset = Immediate();
  if (reg_.status.test(Status::kCarry)) {
    reg_.pc += static_cast<int8_t>(offset);
    // log_elements_.push_back("-> true");
  }
}

void BNE::ExecuteInternal(uint8_t opcode) {
  uint8_t offset = Immediate();
  if (!reg_.status.test(Status::kZero)) {
    reg_.pc += static_cast<int8_t>(offset);
    // log_elements_.push_back("-> true");
  }
}

void BEQ::ExecuteInternal(uint8_t opcode) {
  uint8_t offset = Immediate();
  if (reg_.status.test(Status::kZero)) {
    reg_.pc += static_cast<int8_t>(offset);
    // log_elements_.push_back("-> true");
  }
}
