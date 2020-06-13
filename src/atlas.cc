#include "atlas.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "cpu.h"
#include "memory_impl.h"

Atlas::Atlas(const std::string rom_file) {
  // Open the ROM file as an input stream.
  std::ifstream rom_stream;
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

  // Run until we encounter an error.
  while (cpu_->Run() == Cpu::Status::OK)
    ;
}

Atlas::~Atlas() = default;
