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

  // Mapping of 16K PRG blocks into the 32K address space.
  uint8_t shift_ = 0;
  uint8_t control_ = 0;
  uint8_t chr_bank_0_ = 0;
  uint8_t chr_bank_1_ = 0;
  uint8_t prg_bank_ = 0;
};

#endif  // STORAGE_H_
