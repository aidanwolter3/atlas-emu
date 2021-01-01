#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aTexCoord;

out vec3 TexCoord;

void main() {
  // TODO: set the z coord correctly
  gl_Position = vec4(aPos.xy, 1.0, 1.0);
  TexCoord = aTexCoord;
}
