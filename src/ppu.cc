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

Ppu::Ppu(Cpu& cpu, Window& window)
    : cpu_(cpu),
      window_(window),
      pattern_(2, std::vector<uint8_t>(0x1000, 0)),
      nametable_(4, std::vector<uint8_t>(0x3C0, 0)),
      attribute_(4, std::vector<uint8_t>(0x40, 0)),
      frame_palette_(0x20, 0) {
  // Generate the "unknown tile", which will indicate when a tile has not been
  // "loaded", but needs to be rendered. This "unknown tile" is a red question
  // mark.
  std::vector<uint8_t> unknown_tile;
  for (int i = 0; i < 64; ++i) {
    std::unordered_set<int> question_mark{13, 29, 38, 44, 45};
    if (question_mark.count(i) != 0) {
      unknown_tile.push_back(0x01);
    } else {
      unknown_tile.push_back(0x00);
    }
  }

  // Load the "unknown tile" into the equivalent space for an entire nametable.
  // TODO: We should ideally be able to use the same texture, and instead modify
  // the texture coordinates for the vertices. Optimization opportunity!
  for (int i = 0; i < 960; ++i) {
    window_.SetTile(i, unknown_tile);
  }

  // Load the color palette.
  window_.SetPalette(kColorPalette);
}

Ppu::~Ppu() = default;

void Ppu::Render() {
  // Trigger the NMI if the control bit is set.
  if (ctrl_ & 0x80) {
    cpu_.NMI();
  }

  int table_num = (ctrl_ & 0x03);
  window_.SetAttributeTable(table_num, attribute_[table_num]);
  window_.SetFramePalette(frame_palette_);

  // Load all 960 tiles.
  // TODO: This should be make more efficient. Potentially, we could only load
  // tiles that have changed since the last refresh.
  for (int i = 0; i < (30 * 32); ++i) {
    uint8_t tile_num = nametable_[table_num][i];
    LoadTile(i, tile_num);
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
      if (data_address_ >= 0x4000) {
        std::cout << "ppu error: Address is too large!" << std::endl;
        return Peripheral::Status::OUT_OF_BOUNDS;
      }

      uint16_t tmp_address = data_address_;
      uint8_t* data_location = nullptr;

      // The memory range between 0x3000-0x3EFF are mirrors of 0x2000-0x2EFF.
      if (tmp_address >= 0x3000 && tmp_address <= 0x3EFF) {
        tmp_address -= 0x1000;
      }

      // Pattern table
      if (tmp_address < 0x2000) {
        int table_num = tmp_address / 0x1000;
        tmp_address = tmp_address % 0x1000;
        data_location = &pattern_[table_num][tmp_address];
      }

      // Nametable/Attribute table
      else if (tmp_address < 0x3000) {
        tmp_address -= 0x2000;
        int table_num = tmp_address / 0x400;
        tmp_address = tmp_address % 0x400;
        if (tmp_address < 0x3C0) {
          data_location = &nametable_[table_num][tmp_address];
        } else {
          tmp_address -= 0x3C0;
          data_location = &attribute_[table_num][tmp_address];
        }
      }

      // Frame palette
      else {
        tmp_address -= 0x3F00;
        data_location = &frame_palette_[tmp_address % 0x20];
      }

      *byte = *data_location;
      // std::cout << "ppu read data: " << IntToHexString(data_address_) << "="
      //          << IntToHexString(*byte) << std::endl;
      data_address_ += (ctrl_ & 0x04) ? 32 : 1;
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
        data_address_ = (byte << 8);
      } else {
        data_address_ = (data_address_ & 0xFF00) + byte;
      }
      address_write_to_upper_ = !address_write_to_upper_;
      break;
    case kPpuData:
      if (data_address_ >= 0x4000) {
        std::cout << "ppu error: Address is too large!" << std::endl;
        return Peripheral::Status::OUT_OF_BOUNDS;
      }

      uint16_t tmp_address = data_address_;
      uint8_t* data_location = nullptr;

      // The memory range between 0x3000-0x3EFF are mirrors of 0x2000-0x2EFF.
      if (tmp_address >= 0x3000 && tmp_address <= 0x3EFF) {
        tmp_address -= 0x1000;
      }

      // Pattern table
      if (tmp_address < 0x2000) {
        int table_num = tmp_address / 0x1000;
        tmp_address = tmp_address % 0x1000;
        data_location = &pattern_[table_num][tmp_address];
      }

      // Nametable/Attribute table
      else if (tmp_address < 0x3000) {
        tmp_address -= 0x2000;
        int table_num = tmp_address / 0x400;
        tmp_address = tmp_address % 0x400;
        if (tmp_address < 0x3C0) {
          data_location = &nametable_[table_num][tmp_address];
        } else {
          tmp_address -= 0x3C0;
          data_location = &attribute_[table_num][tmp_address];
        }
      }

      // Frame palette
      else {
        tmp_address -= 0x3F00;
        data_location = &frame_palette_[tmp_address % 0x20];
      }

      *data_location = byte;
      // std::cout << "ppu write data: " << IntToHexString(data_address_) << "="
      //          << IntToHexString(byte) << std::endl;
      data_address_ += (ctrl_ & 0x04) ? 32 : 1;
      break;
  }
  return Peripheral::Status::OK;
}

uint16_t Ppu::GetAddressLength() { return kPpuSize; }

void Ppu::LoadTile(int index, uint8_t tile_num) {
  // TODO: Optimize this so that we are not reloading 960 textures 60 times a
  // second. Potentially, we could keep track of which tiles we have already
  // loaded, and not load them multiple times.

  // std::cout << "LoadTile: index=" << index
  //          << ", tile_num=" << IntToHexString(tile_num)
  //          << ", palette=" << IntToHexString(palette)
  //          << std::endl;

  int pattern_table_num = (ctrl_ >> 4) & 0x01;
  int tile_offset = tile_num * 16;

  std::vector<uint8_t> tile;
  for (int i = 0; i < 8; ++i) {
    uint8_t byte_1 = pattern_[pattern_table_num][tile_offset + i];
    uint8_t byte_2 = pattern_[pattern_table_num][tile_offset + i + 8];
    for (int j = 0; j < 8; ++j) {
      uint8_t color_index = (byte_1 & 0x01) | ((byte_2 & 0x01) << 1);
      tile.insert(tile.begin(), color_index);
      byte_1 = byte_1 >> 1;
      byte_2 = byte_2 >> 1;
    }
  }
  window_.SetTile(index, tile);
}
