#include "atlas.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "cpu.h"
#include "memory_impl.h"

Atlas::Atlas(const std::string rom_file) {
  // Open the ROM file as an input stream.
  std::ifstream rom_stream;
  rom_stream.unsetf(std::ios_base::skipws);
  rom_stream.open(rom_file, std::ios_base::binary);
  if (!rom_stream) {
    std::cout << "Failed to open file: " << rom_file << std::endl;
    return;
  }

  // Collect the data from the ROM.
  std::vector<uint8_t> data;
  std::copy(std::istream_iterator<uint8_t>(rom_stream),
            std::istream_iterator<uint8_t>(), std::back_inserter(data));

  mem_ = std::make_unique<MemoryImpl>(std::move(data));
  cpu_ = std::make_unique<Cpu>(*mem_);
}

Atlas::~Atlas() = default;

Cpu::Status Atlas::Run() {
  Cpu::Status status = Cpu::Status::OK;
  do {
    status = cpu_->Run();
  } while (status == Cpu::Status::OK);
  return status;
}

Cpu::Status Atlas::RunTimes(int times) {
  Cpu::Status status = Cpu::Status::OK;
  for (int i = 0; i < times; ++i) {
    status = cpu_->Run();
    if (status != Cpu::Status::OK) {
      return status;
    }
  }
  return status;
}
