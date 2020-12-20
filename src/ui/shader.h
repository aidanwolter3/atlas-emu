#ifndef SHADER_H_
#define SHADER_H_

#include <string>

#include <glad/glad.h>

class Shader {
 public:
  Shader(GLenum shader_type, std::string file_path);
  ~Shader();

  void AttachToProgram(unsigned int program);

 protected:
  unsigned int shader_;
  unsigned int vbo_;
};

#endif  // SHADER_H_
