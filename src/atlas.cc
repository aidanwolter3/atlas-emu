#include "atlas.h"

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "src/cpu.h"
#include "src/instruction/load.h"
#include "src/instruction/logic.h"
#include "src/instruction/misc.h"
#include "src/instruction/stack.h"
#include "src/instruction/status.h"
#include "src/instruction/store.h"
#include "src/memory.h"

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

  // status
  RegisterInstruction<CLC>(0x18);
  RegisterInstruction<SEC>(0x38);
  RegisterInstruction<CLI>(0x58);
  RegisterInstruction<SEI>(0x78);
  RegisterInstruction<CLV>(0xB8);
  RegisterInstruction<CLD>(0xD8);
  RegisterInstruction<SED>(0xF8);

  // stack
  RegisterInstruction<TXS>(0x9A);
  RegisterInstruction<TSX>(0xBA);
  RegisterInstruction<PHA>(0x48);
  RegisterInstruction<PLA>(0x68);
  RegisterInstruction<PHP>(0x08);
  RegisterInstruction<PLP>(0x28);

  // load / store
  RegisterInstruction<LDA>({0xA9, 0xA5, 0xB5, 0xA1, 0xB1, 0xAD, 0xBD, 0xB9});
  RegisterInstruction<LDX>({0xA2, 0xA6, 0xB6, 0xAE, 0xBE});
  RegisterInstruction<LDY>({0xA0, 0xA4, 0xB4, 0xAC, 0xBC});
  RegisterInstruction<STA>({0x85, 0x95, 0x8D, 0x9D, 0x99, 0x81, 0x91});
  RegisterInstruction<STX>({0x86, 0x96, 0x8E});
  RegisterInstruction<STY>({0x84, 0x94, 0x8C});

  // logic
  RegisterInstruction<AND>({0x29, 0x25, 0x35, 0x2D, 0x3D, 0x39, 0x21, 0x31});
  RegisterInstruction<ORA>({0x09, 0x05, 0x15, 0x0D, 0x1D, 0x19, 0x01, 0x11});
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
