#ifndef MEMORY_H_
#define MEMORY_H_

#include <cstdint>
#include <vector>

class Memory {
 public:
  Memory(std::vector<uint8_t> data);
  ~Memory();

  enum class Status : uint8_t {
    OK,
    OUT_OF_BOUNDS,
    READ_ONLY,
  };

  Memory::Status Read(uint16_t address, uint8_t* byte);
  Memory::Status Write(uint16_t address, uint8_t byte);

 private:
  std::vector<uint8_t> data_;
};

#endif  // MEMORY_H_
