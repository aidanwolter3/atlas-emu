#include "atlas.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "src/cpu.h"
#include "src/instruction/load.h"
#include "src/instruction/status.h"
#include "src/instruction/store.h"
#include "src/memory_impl.h"

Atlas::Atlas(const std::string rom_file) {
  // Open the ROM file as an input stream.
  std::ifstream rom_stream;
  rom_stream.unsetf(std::ios_base::skipws);
  rom_stream.open(rom_file, std::ios_base::binary);
  if (!rom_stream) {
    std::cout << "Failed to open file: " << rom_file << std::endl;
    return;
  }

  // Collect the data from the ROM.
  std::vector<uint8_t> data;
  std::copy(std::istream_iterator<uint8_t>(rom_stream),
            std::istream_iterator<uint8_t>(), std::back_inserter(data));

  mem_ = std::make_unique<MemoryImpl>(std::move(data));
  cpu_ = std::make_unique<Cpu>(*mem_, reg_);
  RegisterInstruction<NOP>(0xEA);
  RegisterInstruction<SEI>(0x78);
  RegisterInstruction<CLD>(0xD8);
  RegisterInstruction<LDA>({0xA9, 0xA5, 0xB5, 0xA1, 0xB1, 0xAD, 0xBD, 0xB9});
  RegisterInstruction<STA>({0x85, 0x95, 0x8D, 0x9D, 0x99, 0x81, 0x91});
}

Atlas::~Atlas() = default;

Cpu::Status Atlas::Run() {
  Cpu::Status status = Cpu::Status::OK;
  do {
    status = cpu_->Run();
  } while (status == Cpu::Status::OK);
  return status;
}

Cpu::Status Atlas::RunTimes(int times) {
  Cpu::Status status = Cpu::Status::OK;
  for (int i = 0; i < times; ++i) {
    status = cpu_->Run();
    if (status != Cpu::Status::OK) {
      return status;
    }
  }
  return status;
}

template <class INS>
void Atlas::RegisterInstruction(uint8_t opcode) {
  RegisterInstruction<INS>(std::vector<uint8_t>({opcode}));
}

template <class INS>
void Atlas::RegisterInstruction(std::vector<uint8_t> opcodes) {
  cpu_->RegisterInstruction(std::make_unique<INS>(*mem_, reg_),
                            std::move(opcodes));
}
