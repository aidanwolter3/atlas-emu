#version 330 core

in vec3 TexCoord;

out vec4 FragColor;

uniform sampler1D palette;
uniform usampler2DArray tiles;
uniform usampler1DArray attributes;
uniform usampler1D palettes;

void main() {
  // Get the tile's x/y coordinates.
  int x = int(TexCoord.x * 32.0);
  int y = int(TexCoord.y * 30.0);

  // Get the attribute for the block.
  int block_index = ((y / 4) * 8) + (x / 4);
  ivec2 block_coord = ivec2(block_index, TexCoord.z);
  uint attr = texelFetch(attributes, block_coord, 0).r;

  // Find the palette/color numbers from the attribute.
  int quad_num_in_block = (((y / 2) % 2) * 2) + ((x / 2) % 2);
  uint palette_offset = (attr >> (quad_num_in_block * 2)) & 3u;
  uint color_num = texture(tiles, TexCoord).r;

  // Grab the correct color from the palette.
  uint palette_num = (palette_offset * 4u) + color_num;
  uint palette_index = texelFetch(palettes, int(palette_num), 0).r;
  FragColor = texelFetch(palette, int(palette_index), 0);
}
