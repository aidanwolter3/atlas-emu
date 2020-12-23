#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <memory>
#include <vector>

class Shader;

class Program {
 public:
  Program(std::vector<std::unique_ptr<Shader>> shaders);
  ~Program();

  // Run a single render refresh.
  void Render();

  const unsigned int vao() { return vao_; }

 private:
  void AddTiles();

  unsigned int program_;
  unsigned int vao_;
  unsigned int vbo_;
  unsigned int ebo_;
  unsigned int tcbo_;

  std::vector<float> vertices_;
  std::vector<unsigned int> elements_;
  // TODO: move this to opengl_window.h
  std::vector<float> texture_coords_;
};

#endif  // PROGRAM_H_