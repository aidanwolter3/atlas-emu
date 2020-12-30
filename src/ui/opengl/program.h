#ifndef UI_OPENGL_PROGRAM_H_
#define UI_OPENGL_PROGRAM_H_

#include <memory>
#include <string>
#include <vector>

#include "src/ui/opengl/shader.h"

class Program {
 public:
  Program(std::vector<std::unique_ptr<Shader>> shaders);
  ~Program();

  // Run a single render refresh.
  void Draw();
  void Use();
  unsigned int GetUniformLocation(std::string name);
  void SetVertices(std::vector<float> vertices);
  void SetElements(std::vector<unsigned int> elements);
  void SetTextureCoords(std::vector<float> coords);

 private:
  unsigned int program_;
  unsigned int vao_;

  // Buffer objects.
  unsigned int vbo_;
  unsigned int ebo_;
  unsigned int tcbo_;

  int element_count_ = 0;
};

#endif  // UI_OPENGL_PROGRAM_H_
