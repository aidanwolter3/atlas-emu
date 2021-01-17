#ifndef ENGINE_MEMORY_H_
#define ENGINE_MEMORY_H_

#include <cstdint>
#include <vector>

#include "src/engine/base/bus.h"

// An interface is helpful for mocking in tests.
class Memory : public Peripheral {};

class MemoryImpl : public Memory {
 public:
  // size*mirror_count amount of data is allocated total. The first |size| bytes
  // are mirrored to adjacent memory |mirror_count| number of times.
  MemoryImpl(uint16_t size, int mirror_count = 1);
  ~MemoryImpl() override;

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;

 private:
  const uint16_t size_;
  const int mirror_count_;
  std::vector<uint8_t> data_;
};

#endif  // ENGINE_MEMORY_H_
