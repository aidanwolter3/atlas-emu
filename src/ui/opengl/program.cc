#include "program.h"

#include <iostream>

// clang-format off
#include <glad/glad.h>  // must go before glfw.h
// clang-format on
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.h"

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
  }

  // Create the vertex arrays.
  // This must be prepared before any objects can be initialized.
  glGenVertexArrays(1, &vao_);

  glUseProgram(program_);
  auto nametable_loc = glGetUniformLocation(program_, "nametable");
  auto attribute_table_loc = glGetUniformLocation(program_, "attribute_table");
  auto frame_palette_loc = glGetUniformLocation(program_, "frame_palette");
  auto palette_loc = glGetUniformLocation(program_, "palette");
  glUniform1i(nametable_loc, 0);
  glUniform1i(attribute_table_loc, 1);
  glUniform1i(frame_palette_loc, 2);
  glUniform1i(palette_loc, 3);

  DrawNametable();
}

Program::~Program() {
  if (program_) {
    glDeleteProgram(program_);
  }
  if (vao_) {
    glDeleteVertexArrays(1, &vao_);
  }
}

void Program::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, elements_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Program::DrawNametable() {
  vertices_ = {
      // nametable 1
      -1.0, 1.0,   // top-left
      1.0, 1.0,    // top-right
      1.0, -1.0,   // bottom-right
      -1.0, -1.0,  // bottom-left

      // nametable 2
      -1.0, -1.0,  // top-left
      1.0, -1.0,   // top-right
      1.0, -3.0,   // bottom-right
      -1.0, -3.0,  // bottom-left

      // nametable 1
      -1.0, -3.0,  // top-left
      1.0, -3.0,   // top-right
      1.0, -5.0,   // bottom-right
      -1.0, -5.0,  // bottom-left
  };
  elements_ = {
      // nametable 1
      0, 1, 2,  // triangle in top-right
      2, 3, 0,  // triangle in bottom-left

      // nametable 2
      4, 5, 6,  // triangle in top-right
      6, 7, 4,  // triangle in bottom-left

      // nametable 1
      8, 9, 10,   // triangle in top-right
      10, 11, 8,  // triangle in bottom-left
  };
  texture_coords_ = {
      // nametable 1
      0.0, 0.0, 0.0,  // bottom-left = tile#0
      1.0, 0.0, 0.0,  // bottom-right
      1.0, 1.0, 0.0,  // top-right
      0.0, 1.0, 0.0,  // top-left

      // nametable 2
      0.0, 0.0, 1.0,  // bottom-left = tile#0
      1.0, 0.0, 1.0,  // bottom-right
      1.0, 1.0, 1.0,  // top-right
      0.0, 1.0, 1.0,  // top-left

      // nametable 1
      0.0, 0.0, 0.0,  // bottom-left = tile#0
      1.0, 0.0, 0.0,  // bottom-right
      1.0, 1.0, 0.0,  // top-right
      0.0, 1.0, 0.0,  // top-left
  };

  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);
  glGenBuffers(1, &tcbo_);
  glBindVertexArray(vao_);

  // Create the vertices.
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float),
               vertices_.data(), GL_STATIC_DRAW);

  // Define the elements (triangles) from the vertex indeces.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_.size() * sizeof(unsigned int),
               elements_.data(), GL_STATIC_DRAW);

  // Add the vertices and elements to the Vertex Array Object (VAO).
  glVertexAttribPointer(/*index=*/0,
                        /*size=*/2,
                        /*type=*/GL_FLOAT,
                        /*normalized=*/GL_FALSE,
                        /*stride=*/2 * sizeof(float),
                        /*offset=*/(void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);

  // Create the texture coordinates.
  glBindBuffer(GL_ARRAY_BUFFER, tcbo_);
  glBufferData(GL_ARRAY_BUFFER, texture_coords_.size() * sizeof(float),
               texture_coords_.data(), GL_STATIC_DRAW);

  // Add the texture coordinates.
  glVertexAttribPointer(/*index=*/1,
                        /*size=*/3,
                        /*type=*/GL_FLOAT,
                        /*normalized=*/GL_FALSE,
                        /*stride=*/3 * sizeof(float),
                        /*offset=*/(void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind to clean up.
  // Note: The vertex array MUST be unbound first.
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
