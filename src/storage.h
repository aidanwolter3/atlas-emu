#ifndef STORAGE_H_
#define STORAGE_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "src/public/bus.h"

// An interface is helpful for mocking in tests.
class Storage : public Peripheral {};

class StorageImpl : public Storage {
 public:
  StorageImpl(std::vector<uint8_t> data);
  ~StorageImpl() override;

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;

 private:
  class Header;

  std::unique_ptr<Header> header_;
  std::vector<uint8_t> prg_;
};

#endif  // STORAGE_H_
