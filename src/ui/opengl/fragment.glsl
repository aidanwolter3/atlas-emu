#version 330 core
in vec3 TexCoord;

out vec4 FragColor;

uniform usampler2DArray tiles;
uniform sampler1D palette;

void main() {
  uint index = texture(tiles, TexCoord).r;
  FragColor = texelFetch(palette, int(index), 0);
}
