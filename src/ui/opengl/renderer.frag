#version 330 core

in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D frame;

void main() {
  frag_color = texture(frame, tex_coord);
}
