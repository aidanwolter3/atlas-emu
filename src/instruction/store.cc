#include "src/instruction/store.h"

#include <cstdint>
#include <iostream>
#include <vector>

void STA::Execute(uint8_t opcode) {
  // Fetch the number of operands.
  std::vector<uint8_t> operands;
  switch (opcode) {
    case 0x85:
    case 0x95:
      operands = FetchOperands(1);
      break;
    case 0x8D:
    case 0x9D:
    case 0x99:
    case 0x81:
    case 0x91:
      operands = FetchOperands(2);
      break;
    default:
      std::cout << "Unsupported STA variant: " << opcode << std::endl;
      return;
  }

  // Grab the address.
  uint16_t address = 0;
  switch (opcode) {
    case 0x8D:
      address = (operands[0] << 8) | operands[1];
      break;
    case 0x85:
    case 0x95:
    case 0x9D:
    case 0x99:
    case 0x81:
    case 0x91:
    default:
      std::cout << "Unsupported LDA variant: " << opcode << std::endl;
      return;
  }

  // Store the data.
  auto status = mem_.Write(address, cpu_.GetAcc());
  if (status != Memory::Status::OK) {
    std::cout << "Failed to write memory at address: " << address << std::endl;
  }
}
