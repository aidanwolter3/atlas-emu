#include "src/engine/ppu.h"

#include "src/engine/base/log.h"
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

int AdjustTableNumForMirroring(int table_num, MirroringMode mode) {
  // TODO: Implement one-screen mirroring.
  switch (mode) {
    case MirroringMode::kVertical:
      return table_num & 0x01;
    case MirroringMode::kHorizontal:
    default:
      return (table_num >> 1) & 0x01;
  }
}

}  // namespace

PpuImpl::PpuImpl(Cpu& cpu, Renderer2& renderer)
    : oam_(0x100, 0),
      pattern_(2, std::vector<uint8_t>(0x1000, 0)),
      nametable_(2, std::vector<uint8_t>(0x3C0, 0)),
      attribute_(2, std::vector<uint8_t>(0x40, 0)),
      frame_palette_(0x20, 0),
      frame_(32 * 30 * 64 * 3, 0),
      cpu_(cpu),
      renderer_(renderer) {}

PpuImpl::~PpuImpl() = default;

void PpuImpl::Tick() {
  // Pixel rendering.
  if (scanline_ < 240) {
    RenderPixel();
  }

  // Start of VBlank.
  if (scanline_ == 241) {
    if (cycle_ == 1) {
      vblank_ = true;
      // Trigger the NMI if the control bit is set.
      if (ctrl_ & 0x80) {
        cpu_.NMI();
      }
    }
  }

  // Prerender.
  if (scanline_ == 261) {
    if (cycle_ == 1) {
      sprite_0_hit_ = false;
      vblank_ = false;
    }
  }

  cycle_++;
  if (cycle_ > 340) {
    cycle_ = 0;
    scanline_++;
  }
  if (scanline_ > 261) {
    renderer_.Render(frame_);
    scanline_ = 0;
  }
}

void PpuImpl::DumpRegisters() {
  LOG(INFO) << "-- PPU --";
  LOG(INFO) << "PPU_CTRL=" << Log::Hex(ctrl_);
  LOG(INFO) << "PPU_MASK=" << Log::Hex(mask_);
  LOG(INFO) << "PPU_OAM_ADDR=" << Log::Hex(oam_address_);
  LOG(INFO) << "PPU_SCROLL_X=" << Log::Hex(scroll_x_);
  LOG(INFO) << "PPU_SCROLL_Y=" << Log::Hex(scroll_y_);
  LOG(INFO) << "PPU_DATA_ADDR=" << Log::Hex(data_address_);
  LOG(INFO) << "---------";
}

void PpuImpl::SetMirroringMode(MirroringMode mode) {
  mirroring_mode_ = mode;
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
        LOG(ERROR) << "ppu error: Address is too large!";
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
      LOG(DEBUG) << "ppu read data: " << Log::Hex(data_address_) << "="
                 << Log::Hex(*byte);
      data_address_ += (ctrl_ & 0x04) ? 32 : 1;
      break;
  }
  LOG(DEBUG) << "ppu read: " << Log::Hex(address) << "=" << Log::Hex(*byte);
  return Peripheral::Status::OK;
}

Peripheral::Status PpuImpl::Write(uint16_t address, uint8_t byte) {
  last_write_value_ = byte;
  LOG(DEBUG) << "ppu write: " << Log::Hex(address) << "=" << Log::Hex(byte);

  // We only have 8 registers.
  address = address % 0x08;

  switch (address) {
    case kPpuStatus:
      return Peripheral::Status::READ_ONLY;
    case kPpuCtrl:
      base_nametable_ = (byte & 0x03);
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
      if (paired_write_latch_) {
        scroll_x_ = byte;
      } else {
        scroll_y_ = byte;
      }
      paired_write_latch_ = !paired_write_latch_;
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
      break;
    case kPpuData:
      if (data_address_ >= 0x4000) {
        LOG(ERROR) << "ppu error: Address is too large!";
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

      *data_location = byte;
      LOG(DEBUG) << "ppu write data: " << Log::Hex(data_address_) << "="
                 << Log::Hex(byte);
      data_address_ += (ctrl_ & 0x04) ? 32 : 1;
      break;
  }
  return Peripheral::Status::OK;
}

uint16_t PpuImpl::GetAddressLength() { return kPpuSize; }

void PpuImpl::RenderPixel() {
  if (cycle_ >= 0x100 || scanline_ >= 0xF0) {
    return;
  }

  auto bg = GetBackgroundPixel();
  auto sp = GetSpritePixel();

  int frame_index = ((scanline_ * 0x100) + cycle_) * 3;

  if (sp.valid && sp.color_num != 0 &&
      (!sp.in_background || bg.color_num == 0)) {
    frame_[frame_index] = sp.r;
    frame_[frame_index + 1] = sp.g;
    frame_[frame_index + 2] = sp.b;
  } else {
    frame_[frame_index] = bg.r;
    frame_[frame_index + 1] = bg.g;
    frame_[frame_index + 2] = bg.b;
  }

  if (!sprite_0_hit_ && sp.valid && sp.is_sprite_zero && sp.color_num != 0 &&
      bg.color_num != 0) {
    sprite_0_hit_ = true;
  }
}

PpuImpl::BackgroundPixel PpuImpl::GetBackgroundPixel() {
  // Calculate the x/y offset into the table.
  int x = (scroll_x_ + cycle_) % 0x100;
  int y = (scroll_y_ + scanline_) % 0xF0;

  int nametable = base_nametable_;
  nametable += (((scroll_x_ + cycle_) % 0x200) / 0x100);
  nametable += (((scroll_y_ + scanline_) % 0x1E0) / 0xF0) * 2;

  int table_num = AdjustTableNumForMirroring(nametable, mirroring_mode_);
  int pattern_table_num = (ctrl_ >> 4) & 0x01;

  // Grab the byte from the nametable.
  int tile_x = x / 8;
  int tile_y = y / 8;
  int tile_num = (tile_y * 32) + tile_x;
  int nametable_byte = nametable_[table_num][tile_num];

  // Grab the pattern from the pattern table.
  int pattern_index = (nametable_byte * 16) + (y % 8);
  int pattern_shift = 7 - (x % 8);
  uint8_t pattern_low = pattern_[pattern_table_num][pattern_index];
  uint8_t pattern_high = pattern_[pattern_table_num][pattern_index + 8];
  uint8_t pattern = ((pattern_high >> pattern_shift) & 0x01) << 1 |
                    ((pattern_low >> pattern_shift) & 0x01);

  // Grab the byte from the attribute table.
  int block_x = x / 32;
  int block_y = y / 32;
  int block_num = (block_y * 8) + block_x;
  int attribute_byte = attribute_[table_num][block_num];

  // Grab the color from the palette using the attribute.
  int attribute_shift =
      (((tile_y >> 1) & 0x01) * 4) + (((tile_x >> 1) & 0x01) * 2);
  int frame_palette_num = (attribute_byte >> attribute_shift) & 0x03;
  int frame_palette_index = frame_palette_num * 4;
  int color_num = frame_palette_[frame_palette_index + pattern];
  int color_index = color_num * 3;

  return {
      .color_num = pattern,
      .r = kColorPalette[color_index],
      .g = kColorPalette[color_index + 1],
      .b = kColorPalette[color_index + 2],
  };
}

PpuImpl::SpritePixel PpuImpl::GetSpritePixel() {
  int x = 0;
  int y = 0;
  int sprite_height = (ctrl_ & 0x20) ? 16 : 8;

  // Select the first sprite that is within range, and find the number as well
  // as the x/y coordinates within the tile.
  int sprite_num;
  for (sprite_num = 0; sprite_num < 64; ++sprite_num) {
    uint8_t sprite_x = oam_[(sprite_num * 4) + 3];
    uint8_t sprite_y = oam_[sprite_num * 4] + 1;

    if ((scanline_ >= sprite_y && scanline_ < sprite_y + sprite_height) &&
        (cycle_ >= sprite_x && cycle_ < sprite_x + 8)) {
      x = cycle_ - sprite_x;
      y = scanline_ - sprite_y;
      break;
    }
  }

  // No sprites found, so return.
  if (sprite_num >= 64) {
    return {};
  }

  // Find the tile and attribute for the sprite.
  uint8_t sprite_tile = oam_[(sprite_num * 4) + 1];
  uint8_t sprite_attribute = oam_[(sprite_num * 4) + 2];

  // Adjust for horizontal flip.
  if (sprite_attribute & 0x40) {
    x = 7 - x;
  }

  // Adjust for vertical flip.
  if (sprite_attribute & 0x80) {
    y = 15 - y;
  }

  // Calculate the pattern table number.
  int pattern_table_num = (ctrl_ >> 2) & 0x01;
  if (ctrl_ & 0x20) {
    // 16-bit sprites get their table num from the first bit of the tile number.
    pattern_table_num = (sprite_tile & 0x01);
    sprite_tile = sprite_tile >> 1;
  }

  // Calculate the index into the pattern table.
  int pattern_index = (sprite_tile * 2 * sprite_height) + y;
  if (y >= 8) {
    // Adjust for 16-bit sprites.
    pattern_index += 8;
  }

  int pattern_shift = 7 - x;
  uint8_t pattern_low = pattern_[pattern_table_num][pattern_index];
  uint8_t pattern_high = pattern_[pattern_table_num][pattern_index + 8];
  uint8_t pattern = ((pattern_high >> pattern_shift) & 0x01) << 1 |
                    ((pattern_low >> pattern_shift) & 0x01);

  int frame_palette_index = ((sprite_attribute & 0x03) + 4) * 4;
  int color_num = frame_palette_[frame_palette_index + pattern];
  int color_index = color_num * 3;

  bool in_background = (sprite_attribute & 0x20) ? true : false;
  return {
      .valid = true,
      .in_background = in_background,
      .is_sprite_zero = sprite_num == 0,
      .color_num = pattern,
      .r = kColorPalette[color_index],
      .g = kColorPalette[color_index + 1],
      .b = kColorPalette[color_index + 2],
  };
}
