#ifndef BUS_IMPL_H_
#define BUS_IMPL_H_

#include <cstdint>
#include <map>
#include <optional>
#include <utility>

#include "src/public/bus.h"

class BusImpl : public Bus {
 public:
  BusImpl();

  // Bus implementation:
  void RegisterPeripheral(Peripheral& peripheral, uint16_t start) override;
  Peripheral::Status Read(uint16_t address, uint8_t* byte) override;
  Peripheral::Status Write(uint16_t address, uint8_t byte) override;
  uint16_t GetAddressLength() override;

 private:
  // Returns the start address of the Peripheral who is reponsible for
  // responding to |address|. The start address can be used to grab the
  // Peripheral from the |peripherals_| map.
  std::optional<uint16_t> FindPeripheralStartForAddress(uint16_t address);

  // Sorted mapping of start address to Peripheral.
  std::map<uint16_t, Peripheral&> peripherals_;
};

#endif  // BUS_IMPL_H_
