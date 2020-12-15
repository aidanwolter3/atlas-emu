#include "atlas.h"

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

#include "src/cpu.h"
#include "src/instruction/branch.h"
#include "src/instruction/compare.h"
#include "src/instruction/jump.h"
#include "src/instruction/load.h"
#include "src/instruction/logic.h"
#include "src/instruction/math.h"
#include "src/instruction/misc.h"
#include "src/instruction/shift.h"
#include "src/instruction/stack.h"
#include "src/instruction/status.h"
#include "src/instruction/store.h"
#include "src/instruction/transfer.h"
#include "src/memory.h"

Atlas::Atlas(const std::string rom_file) : clock_(platform_sleep_) {
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

  mem_ = std::make_unique<MemoryImpl>(/*size=*/0x800, /*mirror_count=*/4);
  mmc1_mem_ = std::make_unique<MemoryImpl>(/*size=*/0x2000);
  mmc1_ = std::make_unique<MMC1Impl>(std::move(data));
  bus_.RegisterPeripheral(*mem_, 0);
  bus_.RegisterPeripheral(*mmc1_mem_, 0x6000);
  bus_.RegisterPeripheral(*mmc1_, 0x8000);
  bus_.RegisterPeripheral(ppu_, 0x2000);
  cpu_ = std::make_unique<Cpu>(event_logger_, clock_, bus_, reg_);

  RegisterInstruction<NOP>(0xEA);
  cpu_->RegisterInstruction(std::make_unique<BRK>(bus_, reg_, event_logger_),
                            {0x00});

  // status
  RegisterInstruction<CLC>(0x18);
  RegisterInstruction<SEC>(0x38);
  RegisterInstruction<CLI>(0x58);
  RegisterInstruction<SEI>(0x78);
  RegisterInstruction<CLV>(0xB8);
  RegisterInstruction<CLD>(0xD8);
  RegisterInstruction<SED>(0xF8);

  // stack
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
  RegisterInstruction<EOR>({0x49, 0x45, 0x55, 0x4D, 0x5D, 0x59, 0x41, 0x51});
  RegisterInstruction<BIT>({0x24, 0x2C});

  // branch
  RegisterInstruction<BPL>(0x10);
  RegisterInstruction<BMI>(0x30);
  RegisterInstruction<BVC>(0x50);
  RegisterInstruction<BVS>(0x70);
  RegisterInstruction<BCC>(0x90);
  RegisterInstruction<BCS>(0xB0);
  RegisterInstruction<BNE>(0xD0);
  RegisterInstruction<BEQ>(0xF0);

  // math
  RegisterInstruction<ADC>({0x69, 0x65, 0x75, 0x6D, 0x7D, 0x79, 0x61, 0x71});
  RegisterInstruction<SBC>({0xE9, 0xE5, 0xF5, 0xED, 0xFD, 0xF9, 0xE1, 0xF1});
  RegisterInstruction<DEC>({0xC6, 0xD6, 0xCE, 0xDE});
  RegisterInstruction<DEX>(0xCA);
  RegisterInstruction<DEY>(0x88);
  RegisterInstruction<INC>({0xE6, 0xF6, 0xEE, 0xFE});
  RegisterInstruction<INX>(0xE8);
  RegisterInstruction<INY>(0xC8);

  // shift
  RegisterInstruction<ASL>({0x0A, 0x06, 0x16, 0x0E, 0x1E});
  RegisterInstruction<LSR>({0x4A, 0x46, 0x56, 0x4E, 0x5E});
  RegisterInstruction<ROL>({0x2A, 0x26, 0x36, 0x2E, 0x3E});
  RegisterInstruction<ROR>({0x6A, 0x66, 0x76, 0x6E, 0x7E});

  // jump
  RegisterInstruction<JMP>({0x4C, 0x6C});
  RegisterInstruction<JSR>(0x20);
  RegisterInstruction<RTS>(0x60);
  RegisterInstruction<RTI>(0x40);

  // compare
  RegisterInstruction<CMP>({0xC9, 0xC5, 0xD5, 0xCD, 0xDD, 0xD9, 0xC1, 0xD1});
  RegisterInstruction<CPX>({0xE0, 0xE4, 0xEC});
  RegisterInstruction<CPY>({0xC0, 0xC4, 0xCC});

  // register
  RegisterInstruction<TAX>(0xAA);
  RegisterInstruction<TXA>(0x8A);
  RegisterInstruction<TAY>(0xA8);
  RegisterInstruction<TYA>(0x98);
  RegisterInstruction<TXS>(0x9A);
  RegisterInstruction<TSX>(0xBA);
}

Atlas::~Atlas() = default;

bool Atlas::Run() {
  while (true) {
    clock_.RunUntilTimer();

    std::optional<EventLogger::Event> error = event_logger_.GetError();
    if (error) {
      event_logger_.PrintLogs();
      return false;
    }

    std::optional<EventLogger::Event> test_result =
        event_logger_.GetTestResult();
    if (test_result) {
      return test_result->type == EventLogger::EventType::kTestPassed;
    }
  }
}

template <class INS>
void Atlas::RegisterInstruction(uint8_t opcode) {
  RegisterInstruction<INS>(std::vector<uint8_t>({opcode}));
}

template <class INS>
void Atlas::RegisterInstruction(std::vector<uint8_t> opcodes) {
  cpu_->RegisterInstruction(std::make_unique<INS>(bus_, reg_),
                            std::move(opcodes));
}
