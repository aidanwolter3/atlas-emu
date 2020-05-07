#ifndef MEMORY_IMPL_H_
#define MEMORY_IMPL_H_

#include <cstdint>
#include <vector>

#include "memory.h"

class MemoryImpl : public Memory {
 public:
  MemoryImpl(std::vector<uint8_t> data);

  Memory::Status Read(uint16_t address, uint8_t* byte) override;
  Memory::Status Write(uint16_t address, uint8_t byte) override;

 private:
  std::vector<uint8_t> data_;
};

#endif  // MEMORY_IMPL_H_
