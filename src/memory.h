#ifndef MEMORY_H_
#define MEMORY_H_

#include <cstdint>

class Memory {
 public:
  virtual ~Memory() {}

  static const uint16_t kHeaderSize = 0x0010;
  static const uint16_t kROMSize = 0x8000;
  static const uint16_t kROMStartAddress = 0x8000;

  enum class Status : uint8_t {
    OK,
    OUT_OF_BOUNDS,
    READ_ONLY,
  };

  virtual Memory::Status Read(uint16_t address, uint8_t* byte) = 0;
  virtual Memory::Status Write(uint16_t address, uint8_t byte) = 0;
};

#endif  // MEMORY_H_
