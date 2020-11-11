#ifndef CPU_H_
#define CPU_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "src/instruction/instruction.h"
#include "src/memory.h"
#include "src/status.h"

class Cpu {
 public:
  Cpu(Memory& mem);
  ~Cpu();

  // TODO: Consider prefixing each instruction with the stage (FETCH, DECODE).
  // TODO: Override the operator<< to print the status name.
  enum class Status : uint8_t {
    OK,
    SEGFAULT,
    UNKNOWN_INSTRUCTION,
  };

  // Run a single clock cycle.
  Cpu::Status Run();

  // Only for testing.
  uint16_t GetPc() { return pc_; }
  StatusRegister GetStatusRegister() { return status_; }
  uint8_t GetAcc() { return acc_; }

 private:
  class CpuProxyImpl : public CpuProxy {
   public:
    CpuProxyImpl(Cpu& cpu) : cpu_(cpu) {}

    // CpuProxy implementation:
    std::vector<uint8_t> FetchOperands(int num) final;
    uint8_t ReadMemoryAtOffset(uint16_t offset) final;
    void WriteMemoryAtOffset(uint16_t offset, uint8_t data) final;
    StatusRegister GetStatusRegister() final;
    void SetStatusRegister(StatusRegister status) final;
    uint8_t GetAcc() final;
    void SetAcc(uint8_t val) final;

   private:
    Cpu& cpu_;
  };

  // Construct and register an instruction with the classname INS that will get
  // executed when the |opcode| is fetched.
  template <class INS>
  void RegisterInstruction(std::vector<uint8_t> opcodes);
  template <class INS>
  void RegisterInstruction(uint8_t opcode);

  // Fetch the |opcode| at |location| in |mem_|.
  Cpu::Status Fetch(uint16_t location, uint8_t* opcode);

  // Registers
  uint16_t pc_;
  StatusRegister status_;
  uint8_t acc_;

  CpuProxyImpl cpu_proxy_;
  std::map<uint8_t, std::shared_ptr<Instruction>> instructions_;
  Memory& mem_;
};

#endif  // CPU_H_
