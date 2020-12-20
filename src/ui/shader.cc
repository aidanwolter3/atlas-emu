#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace {

std::string FileToString(std::string file_path) {
  // TODO: print an error when the file cannot be found.
  std::ifstream s(file_path);
  std::string contents((std::istreambuf_iterator<char>(s)),
                       std::istreambuf_iterator<char>());
  contents.append("\0");
  return contents;
}

}  // namespace

Shader::Shader(GLenum shader_type, std::string file_path)
    : shader_(glCreateShader(shader_type)) {
  if (!shader_) {
    std::cout << "Failed to create the shader: " << file_path << std::endl;
    return;
  }

  // Compile the shader.
  std::string shader_source = FileToString(file_path);
  const char* shader_source_char = shader_source.data();
  glShaderSource(shader_, 1, &shader_source_char, NULL);
  glCompileShader(shader_);

  // Ensure the compilation succeeded.
  int success;
  glGetShaderiv(shader_, GL_COMPILE_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetShaderInfoLog(shader_, 512, NULL, info_log);
    std::cout << "Failed to compile shader: " << file_path << std::endl
              << info_log << std::endl;
    shader_ = 0;
  }
}

Shader::~Shader() {
  if (shader_) {
    glDeleteShader(shader_);
  }
}

void Shader::AttachToProgram(unsigned int program) {
  if (!program) {
    std::cout << "Failed to attach shader" << std::endl;
    return;
  }

  glAttachShader(program, shader_);
}
