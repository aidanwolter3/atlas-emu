#ifndef MEMORY_H_
#define MEMORY_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "src/public/bus.h"

// An interface is helpful for mocking in tests.
class Memory : public Peripheral {};

class MemoryImpl : public Memory {
 public:
  MemoryImpl(std::vector<uint8_t> data);
  ~MemoryImpl() override;

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;

 private:
  class Header;

  std::unique_ptr<Header> header_;
  std::vector<uint8_t> prg_;
};

#endif  // MEMORY_H_
