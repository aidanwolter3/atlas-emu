#version 330 core

in float depth;
in vec3 tex_coord;

out vec4 frag_color;

uniform sampler1D palette;
uniform usampler2DArray tiles;
uniform usampler1DArray attributes;
uniform usampler1D palettes;
uniform bool vertical_mirroring;

void main() {
  // Calculate the texture coordinate inside the appropriate table.
  float table_x = tex_coord.x - floor(tex_coord.x);
  float table_y = tex_coord.y - floor(tex_coord.y);
  
  // Calculate the table number based on the mirroring mode.
  float mirroring_offset = tex_coord.y;
  if (vertical_mirroring) {
    mirroring_offset = tex_coord.x;
  }
  float table_num = floor(mod(mirroring_offset, 2.0));

  // Find the color number (0-3) for the tile.
  uint color_num = texture(tiles, vec3(table_x, table_y, table_num)).r;

  // Zero color pixels should only show up at depth 1.0f.
  // Non-zero color pixels should only show up at depth 0.0f.
  if ((color_num == 0u && depth != 1.0) ||
      (color_num != 0u && depth != 0.0)) {
    discard;
  }

  // Get the tile's x/y coordinates.
  int x = int(table_x * 32.0);
  int y = int(table_y * 30.0);

  // Get the attribute for the block.
  int block_index = ((y / 4) * 8) + (x / 4);
  ivec2 block_coord = ivec2(block_index, table_num);
  uint attr = texelFetch(attributes, block_coord, 0).r;

  // Find the palette/color numbers from the attribute.
  int quad_num_in_block = (((y / 2) % 2) * 2) + ((x / 2) % 2);
  uint palette_offset = (attr >> (quad_num_in_block * 2)) & 3u;

  // Grab the correct color from the palette.
  uint palette_num = (palette_offset * 4u) + color_num;
  uint palette_index = texelFetch(palettes, int(palette_num), 0).r;
  frag_color = texelFetch(palette, int(palette_index), 0);
}
