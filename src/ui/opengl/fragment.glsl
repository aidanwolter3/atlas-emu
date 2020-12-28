#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform usampler2D nametable;
uniform usampler1D attribute_table;
uniform usampler1D frame_palette;
uniform sampler1D palette;

void main() {
  // Get the tile's x/y coordinates.
  int x = int(TexCoord.x * 32.0);
  int y = int(TexCoord.y * 30.0);

  // Get the attribute for the block.
  int block_index = ((y / 4) * 8) + (x / 4);
  uint attribute = texelFetch(attribute_table, block_index, 0).r;

  // Find the palette/color numbers from the attribute.
  int quad_num_in_block = (((y / 2) % 2) * 2) + ((x / 2) % 2);
  uint frame_palette_num = (attribute >> (quad_num_in_block * 2)) & 3u;
  uint color_num = texture(nametable, TexCoord).r;

  // Grab the correct color from the palette.
  uint frame_palette_index = (frame_palette_num * 4u) + color_num;
  uint palette_index = texelFetch(frame_palette, int(frame_palette_index), 0).r;
  FragColor = texelFetch(palette, int(palette_index), 0);
}
