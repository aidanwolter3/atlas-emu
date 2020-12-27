#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aTexCoord;

out vec3 TexCoord;

// Transform for scrolling
uniform mat4 transform;

void main() {
  gl_Position = transform * vec4(aPos.xy, 0.0, 1.0);
  TexCoord = aTexCoord;
}
