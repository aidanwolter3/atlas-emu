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
const uint16_t kOamData = 4;
const uint16_t kPpuScroll = 5;
const uint16_t kPpuAddress = 6;
const uint16_t kPpuData = 7;

// TODO: Move this to a shared utility class.
std::string IntToHexString(int num) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << std::hex << num;
  return ss.str();
}

int AdjustTableNumForMirroring(int table_num, Ppu::MirroringMode mode) {
  // TODO: Implement one-screen mirroring.
  switch (mode) {
    case Ppu::MirroringMode::kVertical:
      return table_num & 0x01;
    case Ppu::MirroringMode::kHorizontal:
    default:
      return table_num >> 1;
  }
}

}  // namespace

PpuImpl::PpuImpl(Cpu& cpu, Renderer& renderer)
    : cpu_(cpu),
      renderer_(renderer),
      oam_(0x100, 0),
      pattern_(2, std::vector<uint8_t>(0x1000, 0)),
      nametable_(2, std::vector<uint8_t>(0x3C0, 0)),
      attribute_(2, std::vector<uint8_t>(0x40, 0)),
      frame_palette_(0x20, 0) {
  renderer_.SetPalette(kColorPalette);
}

PpuImpl::~PpuImpl() = default;

void PpuImpl::Render() {
  // Trigger the NMI if the control bit is set.
  if (ctrl_ & 0x80) {
    cpu_.NMI();
  }

  if (pattern_dirty_ || nametable_dirty_) {
    pattern_dirty_ = nametable_dirty_ = false;
    LoadNametable(0);
    LoadNametable(1);
  }

  if (attribute_dirty_) {
    attribute_dirty_ = false;
    renderer_.SetAttributeTable(0, attribute_[0]);
    renderer_.SetAttributeTable(1, attribute_[1]);
  }

  if (frame_palette_dirty_) {
    frame_palette_dirty_ = false;
    renderer_.SetFramePalette(frame_palette_);
  }

  // TODO: Only set when changed.
  LoadSprites();

  int base_scroll_x = (ctrl_ & 0x01) * 0x100;
  int base_scroll_y = ((ctrl_ >> 1) & 0x01) * 0xF0;
  renderer_.SetScroll(base_scroll_x + scroll_x_, base_scroll_y + scroll_y_);

  renderer_.Render();
}

void PpuImpl::DumpRegisters() {
  std::cout << "-- PPU --" << std::endl;
  std::cout << "PPU_CTRL=" << IntToHexString(ctrl_) << std::endl;
  std::cout << "PPU_MASK=" << IntToHexString(mask_) << std::endl;
  std::cout << "PPU_OAM_ADDR=" << IntToHexString(oam_address_) << std::endl;
  std::cout << "PPU_SCROLL_X=" << IntToHexString(scroll_x_) << std::endl;
  std::cout << "PPU_SCROLL_Y=" << IntToHexString(scroll_y_) << std::endl;
  std::cout << "PPU_DATA_ADDR=" << IntToHexString(data_address_) << std::endl;
  std::cout << "---------" << std::endl;
}

void PpuImpl::SetMirroringMode(MirroringMode mode) { mirroring_mode_ = mode; }

Peripheral::Status PpuImpl::Read(uint16_t address, uint8_t* byte) {
  address = address % 0x08;

  switch (address) {
    case kPpuCtrl:
    case kPpuMask:
    case kOamAddr:
      return Peripheral::Status::WRITE_ONLY;
    case kOamData:
      // TODO: Support OAM reading.
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
      // TODO: Allow the cartridge to redirect the read/writes to
      // cartridge-owned memory.
      else if (tmp_address < 0x3000) {
        tmp_address -= 0x2000;
        int table_num = tmp_address / 0x400;
        table_num = AdjustTableNumForMirroring(table_num, mirroring_mode_);
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

Peripheral::Status PpuImpl::Write(uint16_t address, uint8_t byte) {
  last_write_value_ = byte;
  // std::cout << "ppu write: " << IntToHexString(address) << "="
  //          << IntToHexString(byte) << std::endl;

  // We only have 8 registers.
  address = address % 0x08;

  switch (address) {
    case kPpuStatus:
      return Peripheral::Status::READ_ONLY;
    case kPpuCtrl:
      if ((ctrl_ & 0x03) != (byte & 0x03)) {
        nametable_dirty_ = true;
        attribute_dirty_ = true;
      }
      if ((ctrl_ & 0x10) != (byte & 0x10)) {
        pattern_dirty_ = true;
      }
      ctrl_ = byte;
      break;
    case kPpuMask:
      mask_ = byte;
      break;
    case kOamAddr:
      oam_address_ = byte;
      break;
    case kOamData:
      oam_[oam_address_++] = byte;
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
        pattern_dirty_ = true;
      }

      // Nametable/Attribute table
      else if (tmp_address < 0x3000) {
        tmp_address -= 0x2000;
        int table_num = tmp_address / 0x400;
        table_num = AdjustTableNumForMirroring(table_num, mirroring_mode_);
        tmp_address = tmp_address % 0x400;
        if (tmp_address < 0x3C0) {
          data_location = &nametable_[table_num][tmp_address];
          nametable_dirty_ = true;
        } else {
          tmp_address -= 0x3C0;
          data_location = &attribute_[table_num][tmp_address];
          attribute_dirty_ = true;
        }
      }

      // Frame palette
      else {
        tmp_address -= 0x3F00;
        data_location = &frame_palette_[tmp_address % 0x20];
        frame_palette_dirty_ = true;
      }

      *data_location = byte;
      // std::cout << "ppu write data: " << IntToHexString(data_address_) << "="
      //          << IntToHexString(byte) << std::endl;
      data_address_ += (ctrl_ & 0x04) ? 32 : 1;
      break;
  }
  return Peripheral::Status::OK;
}

uint16_t PpuImpl::GetAddressLength() { return kPpuSize; }

void PpuImpl::LoadNametable(int table_num) {
  constexpr int kBytesPerTile = 64;
  constexpr int kBytesPerRow = kBytesPerTile * 32;
  constexpr int kNametableSize = kBytesPerRow * 30;
  std::vector<uint8_t> nametable(kNametableSize, 0);

  // For every tile...
  int tile_nametable_index = 0;
  for (int row = 0; row < 30; ++row) {
    for (int col = 0; col < 32; ++col) {
      // Grab the tile number from memory...
      int tile_num = nametable_[table_num][tile_nametable_index];

      // For every bit in the tile...
      const int pattern_table_num = (ctrl_ >> 4) & 0x01;
      const int tile_offset = tile_num * 16;
      for (int i = 0; i < 8; ++i) {
        uint8_t byte_1 = pattern_[pattern_table_num][tile_offset + i];
        uint8_t byte_2 = pattern_[pattern_table_num][tile_offset + i + 8];
        for (int j = 0; j < 8; ++j) {
          // Calculate the bit color...
          uint8_t color = (byte_1 & 0x01) | ((byte_2 & 0x01) << 1);

          // And insert the tile data into the nametable.
          int nametable_index =
              (row * kBytesPerRow) + (i * 32 * 8) + (col * 8) + (7 - j);
          nametable[nametable_index] = color;

          byte_1 = byte_1 >> 1;
          byte_2 = byte_2 >> 1;
        }
      }

      tile_nametable_index++;
    }
  }
  renderer_.SetNametable(table_num, nametable);
}

void PpuImpl::LoadSprites() {
  constexpr int kBytesPerSprite = 64;
  constexpr int kNumberOfSprites = 64;
  std::vector<uint8_t> sprite_tiles(kBytesPerSprite * kNumberOfSprites, 0);
  std::vector<Sprite> sprites;

  // For every sprite...
  for (int sprite_num = 0; sprite_num < kNumberOfSprites; ++sprite_num) {
    const int oam_offset = sprite_num * 4;
    const int tile_num = oam_[oam_offset + 1] & 0xFE;
    int pattern_table_num = (ctrl_ >> 3) & 0x01;

    // 8x16 bit tiles are chosen from the pattern table indicated by bit-0 of
    // the tile number in OAM.
    if (ctrl_ & 0x20) {
      pattern_table_num = oam_[oam_offset + 1] & 0x01;
    }

    // For every bit in the tile...
    const int tile_offset = tile_num * 16;
    for (int i = 0; i < 8; ++i) {
      uint8_t byte_1 = pattern_[pattern_table_num][tile_offset + i];
      uint8_t byte_2 = pattern_[pattern_table_num][tile_offset + i + 8];
      for (int j = 0; j < 8; ++j) {
        // Calculate the bit color...
        uint8_t color = (byte_1 & 0x01) | ((byte_2 & 0x01) << 1);

        // And insert the tile data into sprites tiles.
        int index = (sprite_num * 64) + (i * 8) + (7 - j);
        sprite_tiles[index] = color;

        byte_1 = byte_1 >> 1;
        byte_2 = byte_2 >> 1;
      }
    }

    Sprite sprite{
        .x = static_cast<uint8_t>(oam_[oam_offset + 3]),
        // Sprites are all shifted down 1 row for some reason.
        .y = static_cast<uint8_t>(oam_[oam_offset] + 1),
        .tile_num = static_cast<uint8_t>(tile_num),
        .palette = static_cast<uint8_t>(oam_[oam_offset + 2] & 0x03),
    };
    sprites.push_back(sprite);
  }

  renderer_.SetSpriteTiles(sprite_tiles);
  renderer_.SetSprites(sprites);
}
