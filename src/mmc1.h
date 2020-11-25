#ifndef MMC1_H_
#define MMC1_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "src/public/bus.h"

// An interface is helpful for mocking in tests.
class MMC1 : public Peripheral {};

class MMC1Impl : public MMC1 {
 public:
  MMC1Impl(std::vector<uint8_t> data);
  ~MMC1Impl() override;

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

#endif  // MMC1_H_
