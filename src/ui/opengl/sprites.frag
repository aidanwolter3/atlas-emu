#version 330 core

in vec3 TexCoord;

out vec4 FragColor;

uniform sampler1D palette;
uniform usampler2DArray tiles;
uniform usampler1D attributes;
uniform usampler1D palettes;

void main() {
  // Find the palette/color numbers from the attribute.
  uint palette_offset = texelFetch(attributes, int(TexCoord.z), 0).r;
  uint color_num = texture(tiles, TexCoord).r;

  // The zeroth color in sprites is transparent.
  if (color_num == 0u) {
    discard;
  }

  // Grab the correct color from the palette.
  uint palette_num = (palette_offset * 4u) + color_num;
  uint palette_index = texelFetch(palettes, int(palette_num), 0).r;
  FragColor = texelFetch(palette, int(palette_index), 0);
}
