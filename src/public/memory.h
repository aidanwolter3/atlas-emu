#ifndef PUBLIC_MEMORY_H_
#define PUBLIC_MEMORY_H_

#include <cstdint>

class Memory {
 public:
  virtual ~Memory() {}

  enum class Status : uint8_t {
    OK,
    OUT_OF_BOUNDS,
    READ_ONLY,
  };

  virtual Memory::Status Read(uint16_t address, uint8_t* byte) = 0;
  virtual Memory::Status Write(uint16_t address, uint8_t byte) = 0;
};

#endif  // PUBLIC_MEMORY_H_
