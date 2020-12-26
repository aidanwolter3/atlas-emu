#version 330 core
in vec3 TexCoord;

out vec4 FragColor;

uniform usampler2DArray tiles;
uniform usampler1D attribute_table;
uniform usampler1D frame_palette;
uniform sampler1D palette;

void main() {
  // Get the tile's x/y coordinates.
  int i = int(TexCoord.z) / 32;
  int j = int(TexCoord.z) % 32;

  // Get the attribute for the block.
  int block_index = ((i / 4) * 8) + (j / 4);
  uint attribute = texelFetch(attribute_table, block_index, 0).r;

  // Find the palette/color numbers from the attribute.
  int quad_num_in_block = (((i / 2) % 2) * 2) + ((j / 2) % 2);
  uint frame_palette_num = (attribute >> (quad_num_in_block * 2)) & 3u;
  uint color_num = texture(tiles, TexCoord).r;

  // Grab the correct color from the palette.
  uint frame_palette_index = (frame_palette_num * 4u) + color_num;
  uint palette_index = texelFetch(frame_palette, int(frame_palette_index), 0).r;
  FragColor = texelFetch(palette, int(palette_index), 0);
}
