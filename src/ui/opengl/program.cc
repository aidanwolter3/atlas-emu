#include "src/ui/opengl/program.h"

#include <glad/glad.h>
#include <iostream>

namespace {

class ScopedVAOBinding {
 public:
  ScopedVAOBinding(unsigned int vao) { glBindVertexArray(vao); }
  ~ScopedVAOBinding() { glBindVertexArray(0); }
};

}  // namespace

Program::Program(std::vector<std::unique_ptr<Shader>> shaders)
    : program_(glCreateProgram()) {
  if (!program_) {
    std::cout << "Failed to create the program" << std::endl;
    return;
  }

  // Assemble the program.
  for (auto& shader : shaders) {
    shader->AttachToProgram(program_);
  }
  glLinkProgram(program_);

  // Ensure the assembly succeeded.
  int success;
  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetProgramInfoLog(program_, 512, NULL, info_log);
    std::cout << "Failed to compile program" << std::endl
              << info_log << std::endl;
    program_ = 0;
    return;
  }

  // Create the vertex arrays.
  // This must be prepared before any objects can be initialized.
  glGenVertexArrays(1, &vao_);

  // Generate the buffer objects.
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);
  glGenBuffers(1, &tcbo_);
}

Program::~Program() {
  if (program_) {
    glDeleteProgram(program_);
  }
  if (vao_) {
    glDeleteVertexArrays(1, &vao_);
  }
}

void Program::Draw() {
  ScopedVAOBinding vao_binding(vao_);
  glDrawElements(GL_TRIANGLES, element_count_, GL_UNSIGNED_INT, 0);
}

void Program::Use() { glUseProgram(program_); }

unsigned int Program::GetUniformLocation(std::string uniform_name) {
  return glGetUniformLocation(program_, uniform_name.c_str());
}

void Program::SetVertices(std::vector<float> vertices) {
  ScopedVAOBinding vao_binding(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  // Specify the location of the vertices in the VAO.
  glVertexAttribPointer(/*index=*/0,
                        /*size=*/3,
                        /*type=*/GL_FLOAT,
                        /*normalized=*/GL_FALSE,
                        /*stride=*/3 * sizeof(float),
                        /*offset=*/(void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
}

void Program::SetElements(std::vector<unsigned int> elements) {
  ScopedVAOBinding vao_binding(vao_);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int),
               elements.data(), GL_STATIC_DRAW);

  element_count_ = elements.size();
}

void Program::SetTextureCoords(std::vector<float> coords) {
  ScopedVAOBinding vao_binding(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, tcbo_);
  glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(),
               GL_STATIC_DRAW);

  // Specify the location of the texture coordinates in the VAO.
  glVertexAttribPointer(/*index=*/1,
                        /*size=*/3,
                        /*type=*/GL_FLOAT,
                        /*normalized=*/GL_FALSE,
                        /*stride=*/3 * sizeof(float),
                        /*offset=*/(void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(1);
}
