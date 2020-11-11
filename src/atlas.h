#ifndef ATLAS_H_
#define ATLAS_H_

#include <memory>
#include <string>

#include "src/cpu.h"
#include "src/public/registers.h"

class MemoryImpl;

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

  std::unique_ptr<MemoryImpl> mem_;
  Registers reg_;
  std::unique_ptr<Cpu> cpu_;
};

#endif  // ATLAS_H_
