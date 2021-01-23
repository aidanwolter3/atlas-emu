#include "src/engine/engine.h"

#include <optional>
#include <utility>
#include <vector>

#include "src/engine/base/log.h"
#include "src/engine/cpu.h"
#include "src/engine/instruction/branch.h"
#include "src/engine/instruction/compare.h"
#include "src/engine/instruction/jump.h"
#include "src/engine/instruction/load.h"
#include "src/engine/instruction/logic.h"
#include "src/engine/instruction/math.h"
#include "src/engine/instruction/misc.h"
#include "src/engine/instruction/shift.h"
#include "src/engine/instruction/stack.h"
#include "src/engine/instruction/status.h"
#include "src/engine/instruction/store.h"
#include "src/engine/instruction/transfer.h"
#include "src/engine/memory.h"

Engine::Engine(Input& input, Renderer2& renderer, std::vector<uint8_t> rom)
    : oamdma_(bus_), joystick_(input) {
  // Connect all the peripherals to the bus.
  cpu_ = std::make_unique<Cpu>(bus_, reg_);
  mem_ = std::make_unique<MemoryImpl>(/*size=*/0x800, /*mirror_count=*/4);
  ppu_ = std::make_unique<PpuImpl>(*cpu_, renderer);
  mmc1_mem_ = std::make_unique<MemoryImpl>(/*size=*/0x2000);
  mmc1_ = std::make_unique<MMC1Impl>(*ppu_, std::move(rom));
  bus_.RegisterPeripheral(*mem_, 0);
  bus_.RegisterPeripheral(*ppu_, 0x2000);
  bus_.RegisterPeripheral(oamdma_, 0x4014);
  bus_.RegisterPeripheral(joystick_, 0x4016);
  bus_.RegisterPeripheral(*mmc1_mem_, 0x6000);
  bus_.RegisterPeripheral(*mmc1_, 0x8000);

  Reset();
  RegisterInstructions();
}

Engine::RunResult Engine::Run(int num_ticks) {
  RunResult result{
      .num_ticks_ran = 0,
      .can_run = true,
      .has_error = false,
  };

  int ticks;
  for (ticks = 0; ticks < num_ticks; ++ticks) {
    ppu_->Tick();
    ppu_->Tick();
    ppu_->Tick();
    cpu_->Tick();

    if (Log::FoundLogWithLevel(Log::Level::ERROR)) {
      result.can_run = false;
      result.has_error = true;
      break;
    }

    if (Log::FoundLogWithLevel(Log::Level::TEST_FAILED)) {
      result.has_error = true;
      result.can_run = false;
      break;
    }

    if (Log::FoundLogWithLevel(Log::Level::TEST_PASSED)) {
      result.has_error = false;
      result.can_run = false;
      break;
    }
  }
  result.num_ticks_ran = ticks;

  if (result.has_error) {
    DumpState();
  }

  return result;
}

void Engine::Reset() {
  Log::Reset();
  Log::Init(Log::Level::INFO);
  cpu_->Reset();
}

void Engine::DumpState() {
  cpu_->DumpRegisters();
  mmc1_->DumpRegisters();
  ppu_->DumpRegisters();
}

void Engine::RegisterInstructions() {
  RegisterInstruction<NOP>(0xEA);
  RegisterInstruction<BRK>(0x00);

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

  // load
  RegisterInstruction<LDA>(0xA9, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<LDA>(0xA5, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<LDA>(0xB5, Mode::kZeroPageX, Operation::kRead);
  RegisterInstruction<LDA>(0xAD, Mode::kAbsolute, Operation::kRead);
  RegisterInstruction<LDA>(0xBD, Mode::kAbsoluteX, Operation::kRead);
  RegisterInstruction<LDA>(0xB9, Mode::kAbsoluteY, Operation::kRead);
  RegisterInstruction<LDA>(0xA1, Mode::kIndirectX, Operation::kRead);
  RegisterInstruction<LDA>(0xB1, Mode::kIndirectY, Operation::kRead);

  RegisterInstruction<LDX>(0xA2, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<LDX>(0xA6, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<LDX>(0xB6, Mode::kZeroPageY, Operation::kRead);
  RegisterInstruction<LDX>(0xAE, Mode::kAbsolute, Operation::kRead);
  RegisterInstruction<LDX>(0xBE, Mode::kAbsoluteY, Operation::kRead);

  RegisterInstruction<LDY>(0xA0, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<LDY>(0xA4, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<LDY>(0xB4, Mode::kZeroPageX, Operation::kRead);
  RegisterInstruction<LDY>(0xAC, Mode::kAbsolute, Operation::kRead);
  RegisterInstruction<LDY>(0xBC, Mode::kAbsoluteX, Operation::kRead);

  // store
  RegisterInstruction<STA>(0x85, Mode::kZeroPage, Operation::kWrite);
  RegisterInstruction<STA>(0x95, Mode::kZeroPageX, Operation::kWrite);
  RegisterInstruction<STA>(0x8D, Mode::kAbsolute, Operation::kWrite);
  RegisterInstruction<STA>(0x9D, Mode::kAbsoluteX, Operation::kWrite);
  RegisterInstruction<STA>(0x99, Mode::kAbsoluteY, Operation::kWrite);
  RegisterInstruction<STA>(0x81, Mode::kIndirectX, Operation::kWrite);
  RegisterInstruction<STA>(0x91, Mode::kIndirectY, Operation::kWrite);

  RegisterInstruction<STX>(0x86, Mode::kZeroPage, Operation::kWrite);
  RegisterInstruction<STX>(0x96, Mode::kZeroPageY, Operation::kWrite);
  RegisterInstruction<STX>(0x8E, Mode::kAbsolute, Operation::kWrite);

  RegisterInstruction<STY>(0x84, Mode::kZeroPage, Operation::kWrite);
  RegisterInstruction<STY>(0x94, Mode::kZeroPageX, Operation::kWrite);
  RegisterInstruction<STY>(0x8C, Mode::kAbsolute, Operation::kWrite);

  // logic
  RegisterInstruction<EOR>(0x49, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<EOR>(0x45, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<EOR>(0x55, Mode::kZeroPageX, Operation::kRead);
  RegisterInstruction<EOR>(0x4D, Mode::kAbsolute, Operation::kRead);
  RegisterInstruction<EOR>(0x5D, Mode::kAbsoluteX, Operation::kRead);
  RegisterInstruction<EOR>(0x59, Mode::kAbsoluteY, Operation::kRead);
  RegisterInstruction<EOR>(0x41, Mode::kIndirectX, Operation::kRead);
  RegisterInstruction<EOR>(0x51, Mode::kIndirectY, Operation::kRead);

  RegisterInstruction<ORA>(0x09, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<ORA>(0x05, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<ORA>(0x15, Mode::kZeroPageX, Operation::kRead);
  RegisterInstruction<ORA>(0x0D, Mode::kAbsolute, Operation::kRead);
  RegisterInstruction<ORA>(0x1D, Mode::kAbsoluteX, Operation::kRead);
  RegisterInstruction<ORA>(0x19, Mode::kAbsoluteY, Operation::kRead);
  RegisterInstruction<ORA>(0x01, Mode::kIndirectX, Operation::kRead);
  RegisterInstruction<ORA>(0x11, Mode::kIndirectY, Operation::kRead);

  RegisterInstruction<AND>(0x29, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<AND>(0x25, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<AND>(0x35, Mode::kZeroPageX, Operation::kRead);
  RegisterInstruction<AND>(0x2D, Mode::kAbsolute, Operation::kRead);
  RegisterInstruction<AND>(0x3D, Mode::kAbsoluteX, Operation::kRead);
  RegisterInstruction<AND>(0x39, Mode::kAbsoluteY, Operation::kRead);
  RegisterInstruction<AND>(0x21, Mode::kIndirectX, Operation::kRead);
  RegisterInstruction<AND>(0x31, Mode::kIndirectY, Operation::kRead);

  RegisterInstruction<BIT>(0x24, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<BIT>(0x2C, Mode::kAbsolute, Operation::kRead);

  // branch
  RegisterInstruction<BPL>(0x10, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<BMI>(0x30, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<BVC>(0x50, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<BVS>(0x70, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<BCC>(0x90, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<BCS>(0xB0, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<BNE>(0xD0, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<BEQ>(0xF0, Mode::kImmediate, Operation::kRead);

  // math
  RegisterInstruction<ADC>(0x69, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<ADC>(0x65, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<ADC>(0x75, Mode::kZeroPageX, Operation::kRead);
  RegisterInstruction<ADC>(0x6D, Mode::kAbsolute, Operation::kRead);
  RegisterInstruction<ADC>(0x7D, Mode::kAbsoluteX, Operation::kRead);
  RegisterInstruction<ADC>(0x79, Mode::kAbsoluteY, Operation::kRead);
  RegisterInstruction<ADC>(0x61, Mode::kIndirectX, Operation::kRead);
  RegisterInstruction<ADC>(0x71, Mode::kIndirectY, Operation::kRead);

  RegisterInstruction<SBC>(0xE9, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<SBC>(0xE5, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<SBC>(0xF5, Mode::kZeroPageX, Operation::kRead);
  RegisterInstruction<SBC>(0xED, Mode::kAbsolute, Operation::kRead);
  RegisterInstruction<SBC>(0xFD, Mode::kAbsoluteX, Operation::kRead);
  RegisterInstruction<SBC>(0xF9, Mode::kAbsoluteY, Operation::kRead);
  RegisterInstruction<SBC>(0xE1, Mode::kIndirectX, Operation::kRead);
  RegisterInstruction<SBC>(0xF1, Mode::kIndirectY, Operation::kRead);

  RegisterInstruction<DEC>(0xC6, Mode::kZeroPage, Operation::kReadWrite);
  RegisterInstruction<DEC>(0xD6, Mode::kZeroPageX, Operation::kReadWrite);
  RegisterInstruction<DEC>(0xCE, Mode::kAbsolute, Operation::kReadWrite);
  RegisterInstruction<DEC>(0xDE, Mode::kAbsoluteX, Operation::kReadWrite);

  RegisterInstruction<INC>(0xE6, Mode::kZeroPage, Operation::kReadWrite);
  RegisterInstruction<INC>(0xF6, Mode::kZeroPageX, Operation::kReadWrite);
  RegisterInstruction<INC>(0xEE, Mode::kAbsolute, Operation::kReadWrite);
  RegisterInstruction<INC>(0xFE, Mode::kAbsoluteX, Operation::kReadWrite);

  RegisterInstruction<DEX>(0xCA);
  RegisterInstruction<DEY>(0x88);
  RegisterInstruction<INX>(0xE8);
  RegisterInstruction<INY>(0xC8);

  // shift
  RegisterInstruction<ASL>(0x0A, Mode::kImplied, Operation::kReadWrite);
  RegisterInstruction<ASL>(0x06, Mode::kZeroPage, Operation::kReadWrite);
  RegisterInstruction<ASL>(0x16, Mode::kZeroPageX, Operation::kReadWrite);
  RegisterInstruction<ASL>(0x0E, Mode::kAbsolute, Operation::kReadWrite);
  RegisterInstruction<ASL>(0x1E, Mode::kAbsoluteX, Operation::kReadWrite);

  RegisterInstruction<LSR>(0x4A, Mode::kImplied, Operation::kReadWrite);
  RegisterInstruction<LSR>(0x46, Mode::kZeroPage, Operation::kReadWrite);
  RegisterInstruction<LSR>(0x56, Mode::kZeroPageX, Operation::kReadWrite);
  RegisterInstruction<LSR>(0x4E, Mode::kAbsolute, Operation::kReadWrite);
  RegisterInstruction<LSR>(0x5E, Mode::kAbsoluteX, Operation::kReadWrite);

  RegisterInstruction<ROL>(0x2A, Mode::kImplied, Operation::kReadWrite);
  RegisterInstruction<ROL>(0x26, Mode::kZeroPage, Operation::kReadWrite);
  RegisterInstruction<ROL>(0x36, Mode::kZeroPageX, Operation::kReadWrite);
  RegisterInstruction<ROL>(0x2E, Mode::kAbsolute, Operation::kReadWrite);
  RegisterInstruction<ROL>(0x3E, Mode::kAbsoluteX, Operation::kReadWrite);

  RegisterInstruction<ROR>(0x6A, Mode::kImplied, Operation::kReadWrite);
  RegisterInstruction<ROR>(0x66, Mode::kZeroPage, Operation::kReadWrite);
  RegisterInstruction<ROR>(0x76, Mode::kZeroPageX, Operation::kReadWrite);
  RegisterInstruction<ROR>(0x6E, Mode::kAbsolute, Operation::kReadWrite);
  RegisterInstruction<ROR>(0x7E, Mode::kAbsoluteX, Operation::kReadWrite);

  // jump
  RegisterInstruction<JMP>(0x4C, Mode::kImmediateAddress);
  RegisterInstruction<JMP>(0x6C, Mode::kIndirect);
  RegisterInstruction<JSR>(0x20, Mode::kImmediateAddress);
  RegisterInstruction<RTS>(0x60);
  RegisterInstruction<RTI>(0x40);

  // compare
  RegisterInstruction<CMP>(0xC9, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<CMP>(0xC5, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<CMP>(0xD5, Mode::kZeroPageX, Operation::kRead);
  RegisterInstruction<CMP>(0xCD, Mode::kAbsolute, Operation::kRead);
  RegisterInstruction<CMP>(0xDD, Mode::kAbsoluteX, Operation::kRead);
  RegisterInstruction<CMP>(0xD9, Mode::kAbsoluteY, Operation::kRead);
  RegisterInstruction<CMP>(0xC1, Mode::kIndirectX, Operation::kRead);
  RegisterInstruction<CMP>(0xD1, Mode::kIndirectY, Operation::kRead);

  RegisterInstruction<CPX>(0xE0, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<CPX>(0xE4, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<CPX>(0xEC, Mode::kZeroPage, Operation::kRead);

  RegisterInstruction<CPY>(0xC0, Mode::kImmediate, Operation::kRead);
  RegisterInstruction<CPY>(0xC4, Mode::kZeroPage, Operation::kRead);
  RegisterInstruction<CPY>(0xCC, Mode::kAbsolute, Operation::kRead);

  // register
  RegisterInstruction<TAX>(0xAA);
  RegisterInstruction<TXA>(0x8A);
  RegisterInstruction<TAY>(0xA8);
  RegisterInstruction<TYA>(0x98);
  RegisterInstruction<TXS>(0x9A);
  RegisterInstruction<TSX>(0xBA);
}

template <class INS>
void Engine::RegisterInstruction(uint8_t opcode, Mode mode,
                                 Operation operation) {
  // Construct the instruction if needed.
  if (!instructions_.count(opcode)) {
    instructions_[opcode] = std::make_unique<INS>(bus_, reg_);
  }

  cpu_->RegisterInstruction({
      .opcode = opcode,
      .mode = mode,
      .operation = operation,
      .instruction = instructions_[opcode].get(),
  });
}
