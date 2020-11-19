#ifndef PUBLIC_BUS_H_
#define PUBLIC_BUS_H_

#include <cstdint>

class Peripheral {
 public:
  virtual ~Peripheral() {}

  enum class Status : uint8_t {
    OK,
    OUT_OF_BOUNDS,
    READ_ONLY,
  };

  virtual Status Read(uint16_t address, uint8_t* byte) = 0;
  virtual Status Write(uint16_t address, uint8_t byte) = 0;

  virtual uint16_t GetAddressLength() = 0;
};

class Bus : public Peripheral {
 public:
  virtual ~Bus() {}

  virtual void RegisterPeripheral(Peripheral& peripheral, uint16_t start) = 0;
};

#endif  // PUBLIC_BUS_H_
