#ifndef MEMORY_H_
#define MEMORY_H_

#include <vector>

#include "src/public/bus.h"

// An interface is helpful for mocking in tests.
class Memory : public Peripheral {};

class MemoryImpl : public Memory {
 public:
  MemoryImpl();
  ~MemoryImpl() override;

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;

 private:
  std::vector<uint8_t> data_;
};

#endif  // MEMORY_H_