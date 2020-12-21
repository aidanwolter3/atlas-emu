#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

uniform sampler2DArray tiles;

void main() {
  FragColor = texture(tiles, TexCoord);
}
