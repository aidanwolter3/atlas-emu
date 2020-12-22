#include "src/ppu.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "src/ui/color.h"

namespace {

const uint16_t kPpuSize = 0x2000;

// Register offsets.
const uint16_t kPpuCtrl = 0;
const uint16_t kPpuMask = 1;
const uint16_t kPpuStatus = 2;
const uint16_t kOamAddr = 3;
const uint16_t kPpuScroll = 5;
const uint16_t kPpuAddress = 6;
const uint16_t kPpuData = 7;

// TODO: Move this to a shared utility class.
std::string IntToHexString(int num) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << std::hex << num;
  return ss.str();
}

}  // namespace

Ppu::Ppu(Clock& clock, Cpu& cpu, Window& window)
    : cpu_(cpu), window_(window), vram_(0x4000, 0) {
  clock.RegisterTimerObserver(this, MHz(5));

  // Generate the "unknown tile", which will indicate when a tile has not been
  // "loaded", but needs to be rendered. This "unknown tile" is a red question
  // mark.
  std::vector<uint8_t> unknown_tile;
  for (int i = 0; i < 64; ++i) {
    std::unordered_set<int> question_mark{13, 29, 38, 44, 45};
    if (question_mark.count(i) != 0) {
      unknown_tile.insert(unknown_tile.end(), {0xFF, 0x00, 0x00, 0xFF});
    } else {
      unknown_tile.insert(unknown_tile.end(), {0x00, 0x00, 0x00, 0xFF});
    }
  }

  // Load the "unknown tile" into the equivalent space for an entire nametable.
  // TODO: We should ideally be able to use the same texture, and instead modify
  // the texture coordinates for the vertices. Optimization opportunity!
  for (int i = 0; i < 960; ++i) {
    window_.SetTile(i, unknown_tile);
  }
}

Ppu::~Ppu() = default;

Peripheral::Status Ppu::Read(uint16_t address, uint8_t* byte) {
  address = address % 0x08;

  switch (address) {
    case kPpuCtrl:
    case kPpuMask:
    case kOamAddr:
      return Peripheral::Status::WRITE_ONLY;
    case kPpuStatus:
      // For now, we put a dummy value in to get past the Vblank checks at the
      // start of ROMs.
      *byte = 0x80 | (last_write_value_ & 0x1F);
      break;
    case kPpuData:
      // The memory range between 0x3000-0x3EFF are mirrors of 0x2000-0x2EFF.
      uint16_t vram_address = address_;
      if (vram_address >= 0x3000 && vram_address <= 0x3EFF) {
        vram_address -= 0x1000;
      }

      if (vram_address > vram_.size() - 1) {
        std::cout << "ppu error: Address is too large!" << std::endl;
        return Peripheral::Status::OUT_OF_BOUNDS;
      }

      *byte = vram_[vram_address];
      // std::cout << "ppu read data: " << IntToHexString(vram_address) << "="
      //          << IntToHexString(*byte) << std::endl;
      address_ += (ctrl_ & 0x04) ? 32 : 1;
      break;
  }
  // std::cout << "ppu read: " << IntToHexString(address) << "="
  //          << IntToHexString(*byte) << std::endl;
  return Peripheral::Status::OK;
}

Peripheral::Status Ppu::Write(uint16_t address, uint8_t byte) {
  last_write_value_ = byte;
  // std::cout << "ppu write: " << IntToHexString(address) << "="
  //          << IntToHexString(byte) << std::endl;

  // We only have 8 registers.
  address = address % 0x08;

  switch (address) {
    case kPpuStatus:
      return Peripheral::Status::READ_ONLY;
    case kPpuCtrl:
      ctrl_ = byte;
      break;
    case kPpuMask:
      mask_ = byte;
      break;
    case kOamAddr:
      oam_address_ = byte;
      break;
    case kPpuScroll:
      // The first byte written is the X-scroll and the second byte is Y-scroll.
      // After that it continues alternating back and forth.
      if (scroll_write_to_x_) {
        scroll_x_ = byte;
      } else {
        scroll_y_ = byte;
      }
      scroll_write_to_x_ = !scroll_write_to_x_;
      break;
    case kPpuAddress:
      // The first byte written is the upper byte of the address, and the second
      // byte is the lower byte of the address. After that it continues
      // alternating back and forth.
      if (address_write_to_upper_) {
        address_ = (byte << 8);
      } else {
        address_ = (address_ & 0xFF00) + byte;
      }
      address_write_to_upper_ = !address_write_to_upper_;
      break;
    case kPpuData:
      // The memory range between 0x3000-0x3EFF are mirrors of 0x2000-0x2EFF.
      uint16_t vram_address = address_;
      if (vram_address >= 0x3000 && vram_address <= 0x3EFF) {
        vram_address -= 0x1000;
      }

      if (vram_address > vram_.size() - 1) {
        std::cout << "ppu error: Address is too large!" << std::endl;
        return Peripheral::Status::OUT_OF_BOUNDS;
      }

      // std::cout << "ppu write data: " << IntToHexString(vram_address) << "="
      //          << IntToHexString(byte) << std::endl;
      vram_[vram_address] = byte;
      address_ += (ctrl_ & 0x04) ? 32 : 1;
      break;
  }
  return Peripheral::Status::OK;
}

uint16_t Ppu::GetAddressLength() { return kPpuSize; }

void Ppu::OnTimerCalled() {
  // Trigger the NMI if the control bit is set.
  if (ctrl_ & 0x80) {
    cpu_.NMI();
  }

  // Load all 960 tiles.
  // TODO: This should be make more efficient. Potentially, we could only load
  // tiles that have changed since the last refresh.
  const uint16_t nametable = 0x2000 + ((ctrl_ & 0x03) * 0x400);
  const uint16_t attributes = nametable + 0x3C0;
  for (int i = 0; i < 30; ++i) {
    for (int j = 0; j < 32; ++j) {
      // First, find the tile number from the nametable.
      int tile_index = (i * 32) + j;
      uint8_t tile_num = vram_[nametable + tile_index];

      // Second, find the palette from the attributes.
      int block_index = (((i / 4) * 8) + (j / 4));
      uint8_t palette = vram_[attributes + block_index];

      // Third, calculate which quad of the block the tile is in.
      // This will be between 0 and 3.
      int quad_num_in_block = ((j / 2) % 2) + (((i / 2) % 2) * 2);

      // Fourth, take the 2-bits that correlate to that quad.
      // The result will be between 0 and 3.
      palette = (palette >> (quad_num_in_block * 2)) & 0x03;

      // Finally, load the tile using these findings.
      LoadTile(tile_index, tile_num, palette);
    }
  }

  window_.Update();

  // std::cout << "NAMETABLE" << std::endl;
  // for (int i = 0; i < 30; ++i) {
  //  for (int j = 0; j < 32; ++j) {
  //    int index = i * 32 + j + 0x2000;
  //    std::cout << " " << IntToHexString(vram_[index]);
  //  }
  //  std::cout << std::endl;
  //}
}

void Ppu::LoadTile(int index, uint8_t tile_num, uint8_t palette) {
  // TODO: Optimize this so that we are not reloading 960 textures 60 times a
  // second. Potentially, we could keep track of which tiles we have already
  // loaded, and not load them multiple times.

  // std::cout << "LoadTile: index=" << index
  //          << ", tile_num=" << IntToHexString(tile_num)
  //          << ", palette=" << IntToHexString(palette)
  //          << std::endl;

  uint16_t palette_offset = 0x3F00 + (palette * 4);
  uint8_t color_0 = vram_[palette_offset];
  uint8_t color_1 = vram_[palette_offset + 1];
  uint8_t color_2 = vram_[palette_offset + 2];
  uint8_t color_3 = vram_[palette_offset + 3];

  int tile_index = tile_num * 16;
  if (ctrl_ & 0x10) {
    tile_index += 0x1000;
  }

  std::vector<uint8_t> tile;
  for (int i = 0; i < 8; ++i) {
    uint8_t byte_1 = vram_[tile_index + i];
    uint8_t byte_2 = vram_[tile_index + i + 8];
    for (int j = 0; j < 8; ++j) {
      uint8_t color = (byte_1 & 0x01) | ((byte_2 & 0x01) << 1);
      std::vector<uint8_t> pixel{0x00, 0x00, 0x00, 0xFF};
      switch (color) {
        case 0x00:
          pixel = Color(color_0);
          break;
        case 0x01:
          pixel = Color(color_1);
          break;
        case 0x02:
          pixel = Color(color_2);
          break;
        case 0x03:
          pixel = Color(color_3);
          break;
        default:
          break;
      }
      tile.insert(tile.begin(), pixel.begin(), pixel.end());
      byte_1 = byte_1 >> 1;
      byte_2 = byte_2 >> 1;
    }
  }
  window_.SetTile(index, tile);
}
