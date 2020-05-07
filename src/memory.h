#ifndef MEMORY_H_
#define MEMORY_H_

#include <cstdint>
#include <vector>

class Memory {
 public:
  Memory(std::vector<uint8_t> data);
  ~Memory();

  uint8_t Read(uint16_t address);
  uint8_t Write(uint16_t address);

 private:
  std::vector<uint8_t> data_;
};

#endif  // MEMORY_H_
