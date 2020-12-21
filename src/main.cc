#include <iostream>

#include "src/atlas.h"

int main(int argc, char** argv) {
  std::cout << "Welcome to Atlas!" << std::endl;

  std::string rom_path;
  if (argc < 2) {
    std::cout << "Missing ROM argument" << std::endl;
    return 1;
  }
  rom_path = argv[1];
  std::cout << "Loading ROM: " << rom_path << std::endl;

  Atlas atlas(rom_path);
  atlas.Run();
  return 0;
}
