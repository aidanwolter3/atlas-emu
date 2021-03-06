#ifndef ENGINE_MMC1_H_
#define ENGINE_MMC1_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "src/engine/base/bus.h"
#include "src/engine/ppu.h"

// An interface is helpful for mocking in tests.
class MMC1 : public Peripheral {};

class MMC1Impl : public MMC1 {
 public:
  MMC1Impl(Ppu& ppu, std::vector<uint8_t> data);
  ~MMC1Impl() override;

  void DumpRegisters();

  // Peripheral implementation:
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;

 private:
  class Header;

  Ppu& ppu_;
  std::unique_ptr<Header> header_;
  std::vector<uint8_t> prg_;

  // Mapping of 16K PRG blocks into the 32K address space.
  uint8_t shift_ = 0;
  uint8_t control_ = 0;
  uint8_t chr_bank_0_ = 0;
  uint8_t chr_bank_1_ = 0;
  uint8_t prg_bank_ = 0;
};

#endif  // ENGINE_MMC1_H_
