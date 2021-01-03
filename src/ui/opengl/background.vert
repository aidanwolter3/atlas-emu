#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_tex_coord;

out float depth;
out vec3 tex_coord;

// Transform for scrolling
uniform mat4 transform;

void main() {
  gl_Position = transform * vec4(in_position , 1.0);
  depth = in_position.z;
  tex_coord = in_tex_coord;
}
