#ifndef ATLAS_H_
#define ATLAS_H_

#include <memory>
#include <string>

#include "cpu.h"

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
  std::unique_ptr<MemoryImpl> mem_;
  std::unique_ptr<Cpu> cpu_;
};

#endif  // ATLAS_H_
