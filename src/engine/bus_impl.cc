#include "src/engine/bus_impl.h"

#include <iostream>

namespace {

// TODO: Consider using templates to instantiate Buses of different sizes.
const uint32_t kBusSize = 0x10000;

}  // namespace

BusImpl::BusImpl() = default;

void BusImpl::RegisterPeripheral(Peripheral& peripheral, uint16_t start) {
  if ((kBusSize - start) < peripheral.GetAddressLength()) {
    std::cout << "Cannot register peripheral: "
              << "address range exceeds the bus address range." << std::endl;
    return;
  }

  // If there are no other peripherals, insert without checking address
  // collisions.
  if (peripherals_.size() == 0) {
    peripherals_.emplace(start, peripheral);
    return;
  }

  // Another peripheral has the same start range, so we can quickly determine
  // that the addresse ranges collide.
  if (peripherals_.count(start) != 0) {
    std::cout << "Cannot register peripheral: "
              << "address range overlaps another peripheral." << std::endl;
    return;
  }

  // Find an existing Peripheral whose start address is immediately smaller than
  // |start|. We could speed this up with a binary-search, but in reality, we
  // will not have many Peripherals in our map.
  auto lower_it = peripherals_.end();
  for (auto it = peripherals_.begin(); it != peripherals_.end(); ++it) {
    if (start < it->first) {
      break;
    }
    lower_it = it;
  }

  // Check if the lower Peripheral has an overlapping address range.
  if (lower_it != peripherals_.end()) {
    if (start < lower_it->first + lower_it->second.GetAddressLength()) {
      std::cout << "Cannot register peripheral: "
                << "address range overlaps another peripheral." << std::endl;
      return;
    }
  }

  // Check if the upper Peripheral has an overlapping address range.
  auto upper_it = peripherals_.begin();
  if (lower_it != peripherals_.end()) {
    upper_it = lower_it;
    upper_it++;
  }
  if (upper_it != peripherals_.end()) {
    if (start + peripheral.GetAddressLength() > upper_it->first) {
      std::cout << "Cannot register peripheral: "
                << "address range overlaps another peripheral." << std::endl;
      return;
    }
  }

  // The address range does not collide with any other Peripherals, so insert
  // the Peripheral into the mapping.
  peripherals_.emplace(start, peripheral);
}

Peripheral::Status BusImpl::Read(uint16_t address, uint8_t* byte) {
  std::optional<uint16_t> start = FindPeripheralStartForAddress(address);
  if (!start) {
    return Peripheral::Status::OUT_OF_BOUNDS;
  }

  Peripheral& peripheral = peripherals_.at(*start);
  return peripheral.Read(address - *start, byte);
}

Peripheral::Status BusImpl::Write(uint16_t address, uint8_t byte) {
  std::optional<uint16_t> start = FindPeripheralStartForAddress(address);
  if (!start) {
    return Peripheral::Status::OUT_OF_BOUNDS;
  }

  Peripheral& peripheral = peripherals_.at(*start);
  return peripheral.Write(address - *start, byte);
}

uint16_t BusImpl::GetAddressLength() {
  // We cannot return the real bus size, because uint16_t's max value is 1 less
  // than the size. Currently, no one uses the Bus's size. Return 0, to make it
  // obvious in the future if anyone tries to use it.
  return 0;
}

std::optional<uint16_t> BusImpl::FindPeripheralStartForAddress(
    uint16_t address) {
  for (auto it : peripherals_) {
    if (address >= it.first &&
        address < it.first + it.second.GetAddressLength()) {
      return it.first;
    }
  }
  return std::nullopt;
}
