#ifndef ATLAS_H_
#define ATLAS_H_

#include <memory>
#include <string>

#include "src/bus_impl.h"
#include "src/cpu.h"
#include "src/memory.h"
#include "src/mmc1.h"
#include "src/ppu.h"
#include "src/public/registers.h"

class Atlas {
 public:
  Atlas(const std::string rom_file);
  ~Atlas();

  // Run the CPU until an error occurs.
  Cpu::Status Run();

  // Run the CPU |times| clock cycles or until an error occurs.
  Cpu::Status RunTimes(int times);

 private:
  // Construct and register an instruction with the classname INS that will get
  // executed when |opcode| is fetched.
  template <class INS>
  void RegisterInstruction(uint8_t opcode);
  // Construct and register an instruction with the classname INS that will get
  // executed when one of the |opcodes| are fetched.
  template <class INS>
  void RegisterInstruction(std::vector<uint8_t> opcodes);

  BusImpl bus_;
  MemoryImpl mem_;
  Registers reg_;
  Ppu ppu_;
  std::unique_ptr<MMC1Impl> mmc1_;
  std::unique_ptr<Cpu> cpu_;
};

#endif  // ATLAS_H_
