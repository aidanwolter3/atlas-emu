#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tile;

void main() {
  FragColor = texture(tile, TexCoord);
}
