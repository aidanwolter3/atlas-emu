#include "src/engine/ppu.h"

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

int AdjustTableNumForMirroring(int table_num, MirroringMode mode) {
  // TODO: Implement one-screen mirroring.
  switch (mode) {
    case MirroringMode::kVertical:
      return table_num & 0x01;
    case MirroringMode::kHorizontal:
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
      frame_palette_(0x20, 0),
      background_(2, std::vector<uint8_t>(64 * 32 * 30, 0)) {
  renderer_.SetPalette(kColorPalette);
}

PpuImpl::~PpuImpl() = default;

void PpuImpl::Scanline() {
  // Reset the vertical split when rendering starts.
  if (scanline_ == 0) {
    vertical_split_scanline_ = 0;
    vertical_split_base_nametable_ = base_nametable_;
    vertical_split_scroll_x_ = scroll_x_;
    vertical_split_scroll_y_ = scroll_y_;
  }

  // Normal rendering.
  if (scanline_ < 240) {
    // Detect if sprite 0 has a hit anywhere on this scanline.
    // TODO: Should the CPU be able to run in the middle of a scanline?
    for (int x = 0; !sprite_0_hit_ && x < 0xFF; ++x) {
      DetectSprite0HitAtCoordinate(x, scanline_);
    }
  }

  // VBlank.
  else if (scanline_ == 241) {
    vblank_ = true;

    // Trigger the NMI if the control bit is set.
    if (ctrl_ & 0x80) {
      cpu_.NMI();
    }
  }

  // Pre-rendering.
  else if (scanline_ == 261) {
    vblank_ = false;
    sprite_0_hit_ = false;
    scanline_ = -1;
  }

  scanline_++;
}

void PpuImpl::Render() {
  if (pattern_dirty_ || nametable_dirty_) {
    pattern_dirty_ = nametable_dirty_ = false;
    LoadBackground();
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

  if (oam_dirty_) {
    oam_dirty_ = false;
    LoadSprites();
  }

  int base_scroll_x = (base_nametable_ & 0x01) * 0x100;
  int base_scroll_y = ((base_nametable_ >> 1) & 0x01) * 0xF0;
  renderer_.SetScroll(base_scroll_x + scroll_x_, base_scroll_y + scroll_y_);

  int split_base_scroll_x = (vertical_split_base_nametable_ & 0x01) * 0x100;
  int split_base_scroll_y =
      ((vertical_split_base_nametable_ >> 1) & 0x01) * 0xF0;
  renderer_.SetVerticalSplit(vertical_split_scanline_,
                             split_base_scroll_x + vertical_split_scroll_x_,
                             split_base_scroll_y + vertical_split_scroll_y_);

  renderer_.SetMask(mask_);
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

void PpuImpl::SetMirroringMode(MirroringMode mode) {
  mirroring_mode_ = mode;
  renderer_.SetMirroringMode(mode);
}

Peripheral::Status PpuImpl::Read(uint16_t address, uint8_t* byte) {
  address = address % 0x08;

  uint8_t status = last_write_value_ & 0x1F;
  switch (address) {
    case kPpuCtrl:
    case kPpuMask:
    case kOamAddr:
      return Peripheral::Status::WRITE_ONLY;
    case kOamData:
      *byte = oam_[oam_address_++];
      return Peripheral::Status::OK;
    case kPpuStatus:
      if (vblank_) {
        status |= 0x80;
        vblank_ = false;
      }
      if (sprite_0_hit_) {
        status |= 0x40;
      }
      *byte = status;
      paired_write_latch_ = true;
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
      if (base_nametable_ != (byte & 0x03)) {
        base_nametable_ = (byte & 0x03);
        nametable_dirty_ = true;
        attribute_dirty_ = true;

        // Nametable is modified during the visible scanlines, which means a
        // split is occuring.
        if (scanline_ < 241) {
          vertical_split_scanline_ = scanline_ + 1;
          // TODO: We write the x/y values here, because otherwise, the scrolly
          // will not be adjusted for the current scanline. We should
          // investigate a better approach, as this will not work when the
          // PpuAddr is changed, then the base nametable is changed mid-frame.
          vertical_split_base_nametable_ = base_nametable_;
          vertical_split_scroll_y_ = scroll_y_ + vertical_split_scanline_;
        }
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
      oam_dirty_ = true;
      break;
    case kPpuScroll:
      // The first byte written is the X-scroll and the second byte is Y-scroll.
      // After that it continues alternating back and forth.
      if (paired_write_latch_) {
        scroll_x_ = byte;
      } else {
        scroll_y_ = byte;
      }
      paired_write_latch_ = !paired_write_latch_;

      // Scroll is modified during the visible scanlines, which means a split
      // is occuring.
      if (scanline_ < 241) {
        vertical_split_scanline_ = scanline_ + 1;
        vertical_split_scroll_x_ = scroll_x_;
        vertical_split_scroll_y_ = scroll_y_ + vertical_split_scanline_;
      }
      break;
    case kPpuAddress:
      // The first byte written is the upper byte of the address, and the second
      // byte is the lower byte of the address. After that it continues
      // alternating back and forth.
      if (paired_write_latch_) {
        data_address_ = (byte << 8);
      } else {
        data_address_ = (data_address_ & 0xFF00) + byte;
      }

      // The PPU shared registers for several purposes, therefore this affects
      // other variables.
      if (paired_write_latch_) {
        base_nametable_ = ((byte & 0x0C) >> 2);
        scroll_y_ &= 0b00111000;
        scroll_y_ |= ((byte & 0x03) << 6) | ((byte & 0x30) >> 4);
      } else {
        scroll_y_ &= 0b11000111;
        scroll_y_ |= ((byte & 0xE0) >> 2);
        scroll_x_ &= 0b00000111;
        scroll_x_ |= ((byte & 0x1F) << 3);
      }
      paired_write_latch_ = !paired_write_latch_;

      // Address is modified during the visible scanlines, which means a split
      // is occuring.
      if (scanline_ < 241) {
        vertical_split_scanline_ = scanline_ + 1;
        vertical_split_base_nametable_ = base_nametable_;
        vertical_split_scroll_x_ = scroll_x_;
        vertical_split_scroll_y_ = scroll_y_;
      }
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

void PpuImpl::LoadBackground() {
  constexpr int kBytesPerTile = 64;
  constexpr int kBytesPerRow = kBytesPerTile * 32;
  for (int table_num = 0; table_num < 2; ++table_num) {
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
            int background_index =
                (row * kBytesPerRow) + (i * 32 * 8) + (col * 8) + (7 - j);
            background_[table_num][background_index] = color;

            byte_1 = byte_1 >> 1;
            byte_2 = byte_2 >> 1;
          }
        }

        tile_nametable_index++;
      }
    }
    renderer_.SetNametable(table_num, background_[table_num]);
  }
}

void PpuImpl::LoadSprites() {
  constexpr int kNumberOfSprites = 64;
  std::vector<Sprite> sprites;

  // For every sprite...
  for (int sprite_num = 0; sprite_num < kNumberOfSprites; ++sprite_num) {
    const int oam_offset = sprite_num * 4;
    int pattern_table_num = (ctrl_ >> 3) & 0x01;
    int tile_num = oam_[oam_offset + 1];
    int height = (ctrl_ & 0x20) ? 16 : 8;

    // 8x16 bit tiles are chosen from the pattern table indicated by bit-0 of
    // the tile number in OAM.
    if (height == 16) {
      pattern_table_num = oam_[oam_offset + 1] & 0x01;
      tile_num &= 0xFE;
    }

    // For every bit in the tile...
    std::vector<uint8_t> sprite_tile(8 * height, 0);
    for (int i = 0; i < height; ++i) {
      int tile_index = 0;
      if (i < 8) {
        tile_index = (tile_num * 16) + i;
      } else {
        tile_index = (tile_num * 16) + i + 8;
      }

      uint8_t byte_1 = pattern_[pattern_table_num][tile_index];
      uint8_t byte_2 = pattern_[pattern_table_num][tile_index + 8];
      for (int j = 0; j < 8; ++j) {
        // Calculate the bit color...
        uint8_t color = (byte_1 & 0x01) | ((byte_2 & 0x01) << 1);

        // And insert the tile data into sprites tiles.
        int index = (i * 8) + (7 - j);
        sprite_tile[index] = color;

        byte_1 = byte_1 >> 1;
        byte_2 = byte_2 >> 1;
      }
    }

    Sprite sprite{
        .x = oam_[oam_offset + 3],
        // Sprites are all shifted down 1 row for some reason.
        .y = static_cast<uint8_t>(oam_[oam_offset] + 1),
        .attribute = oam_[oam_offset + 2],
        // TODO: this may cause a copy of the sprite_tile.
        // Maybe we should have a move ctor for Sprite.
        .tile = sprite_tile,
    };
    sprites.push_back(sprite);

    // Save the first sprite for hit detection.
    if (sprite_num == 0) {
      sprite_0_ = sprite;
    }
  }

  renderer_.SetSprites(sprites);
}

void PpuImpl::DetectSprite0HitAtCoordinate(int x, int y) {
  if (!sprite_0_ || sprite_0_->y >= 0xF0) {
    return;
  }

  // Calculate the offset into the sprite's tile.
  int sprite_x = x - sprite_0_->x;
  int sprite_y = y - sprite_0_->y;

  // Sprite does not overlap with (x, y).
  int sprite_height = sprite_0_->tile.size() / 8;
  if ((sprite_x < 0 || sprite_x >= 8) ||
      (sprite_y < 0 || sprite_y >= sprite_height)) {
    return;
  }

  // Find the sprite's color at (x, y).
  int tile_index = (sprite_x * 8) + sprite_y;
  uint8_t sprite_color = sprite_0_->tile[tile_index];

  // Return early, if the sprite's color is zero.
  if (sprite_color == 0) {
    return;
  }

  // Calculate the origin of the background after the scroll, accounting for the
  // base nametable.
  int bg_origin_x = ((base_nametable_ & 0x01) * 0x100) + scroll_x_;
  int bg_origin_y = ((base_nametable_ >> 1) & 0x01) * 0xF0 + scroll_y_;

  // Calculate which table is at (x, y), accounting for the mirroring mode.
  int table_num = 0;
  if (mirroring_mode_ == MirroringMode::kHorizontal) {
    table_num = (bg_origin_y % 0x1E0) / 0xF0;
  } else if (mirroring_mode_ == MirroringMode::kVertical) {
    table_num = (bg_origin_x % 0x200) / 0x100;
  } else {
    std::cout << "Unsupported mirroring mode" << std::endl;
  }

  // Calculate the offset into the background.
  int bg_x = (bg_origin_x % 0xF0) + x;
  int bg_y = (bg_origin_y % 0xF0) + y;

  // Find the background's color at (x, y).
  int bg_index = (bg_y * 0xFF) + bg_x;
  uint8_t bg_color = background_[table_num][bg_index];

  // Return if the background's color is zero.
  if (bg_color == 0) {
    return;
  }

  // Set hit, because both colors are non-zero.
  sprite_0_hit_ = true;
}
