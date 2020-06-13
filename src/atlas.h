#ifndef ATLAS_H_
#define ATLAS_H_

#include <memory>
#include <string>

class Cpu;
class MemoryImpl;

class Atlas {
 public:
  Atlas(const std::string rom_file);
  ~Atlas();

 private:
  std::unique_ptr<MemoryImpl> mem_;
  std::unique_ptr<Cpu> cpu_;
};

#endif  // ATLAS_H_
