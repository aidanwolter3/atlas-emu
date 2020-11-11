#include "src/instruction/load.h"

#include <cstdint>
#include <iostream>
#include <vector>

void LDA::Execute(uint8_t opcode) {
  // Fetch the number of operands.
  std::vector<uint8_t> operands;
  switch (opcode) {
    case 0xA9:
    case 0xA5:
    case 0xB5:
    case 0xA1:
    case 0xB1:
      operands = FetchOperands(1);
      break;
    case 0xAD:
    case 0xBD:
    case 0xB9:
      operands = FetchOperands(2);
      break;
    default:
      std::cout << "Unsupported LDA variant: " << opcode << std::endl;
      return;
  }

  // Grab the data.
  uint8_t data = 0;
  switch (opcode) {
    case 0xA9:
      data = operands[0];
      break;
    case 0xA5:
      mem_.Read(operands[0], &data);
      break;
    case 0xB5:
    case 0xA1:
    case 0xB1:
    case 0xAD:
    case 0xBD:
    case 0xB9:
    default:
      std::cout << "Unsupported LDA variant: " << opcode << std::endl;
      return;
  }

  // Set the registers.
  cpu_.SetAcc(data);
  auto s = cpu_.GetStatusRegister();
  s.zero = (data == 0);
  s.sign = (static_cast<int8_t>(data) < 0);
  cpu_.SetStatusRegister(s);
}
