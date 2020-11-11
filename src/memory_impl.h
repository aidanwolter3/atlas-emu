#ifndef MEMORY_IMPL_H_
#define MEMORY_IMPL_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "src/public/memory.h"

class MemoryImpl : public Memory {
 public:
  MemoryImpl(std::vector<uint8_t> data);
  ~MemoryImpl();

  Memory::Status Read(uint16_t address, uint8_t* byte) override;
  Memory::Status Write(uint16_t address, uint8_t byte) override;

 private:
  class Header;

  std::unique_ptr<Header> header_;
  std::vector<uint8_t> prg_;
};

#endif  // MEMORY_IMPL_H_
