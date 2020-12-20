#include "program.h"

#include <iostream>
#include <unordered_set>

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

  AddTiles();
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

  glUseProgram(program_);

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, elements_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Program::AddTiles() {
  const int width = 32;
  const int height = 30;
  const float tile_unit_x = 2.0 / width;
  const float tile_unit_y = 2.0 / height;

  // Populate the vertices_/elements_/texture_coords_ vectors, starting from
  // the top-left, and filling each row before moving down.
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      // Add the vertices.
      const float top_left_x = -1.0 + (tile_unit_x * col);
      const float top_left_y = 1.0 - (tile_unit_y * row);
      vertices_.insert(vertices_.end(), {
                                            top_left_x,
                                            top_left_y,
                                            0.0,
                                            top_left_x + tile_unit_x,
                                            top_left_y,
                                            0.0,
                                            top_left_x + tile_unit_x,
                                            top_left_y - tile_unit_y,
                                            0.0,
                                            top_left_x,
                                            top_left_y - tile_unit_y,
                                            0.0,
                                        });

      // Add the elements.
      const unsigned int top_left =
          ((row * width) + col) * 4;  // tile_num * num_vertices_per_tile
      const unsigned int top_right = top_left + 1;
      const unsigned int bottom_right = top_left + 2;
      const unsigned int bottom_left = top_left + 3;
      elements_.insert(
          elements_.end(),
          {
              top_left, top_right, bottom_right,    // triangle in top-right
              bottom_right, bottom_left, top_left,  // triangle in bottom-left
          });

      // Add the texture coordinates.
      texture_coords_.insert(texture_coords_.end(),
                             {
                                 0.0f, 1.0f,  // top left
                                 1.0f, 1.0f,  // top right
                                 1.0f, 0.0f,  // bottom right
                                 0.0f, 0.0f,  // bottom left
                             });
    }
  }

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
                        /*size=*/3,
                        /*type=*/GL_FLOAT,
                        /*normalized=*/GL_FALSE,
                        /*stride=*/3 * sizeof(float),
                        /*offset=*/(void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);

  // Create the texture coordinates.
  glBindBuffer(GL_ARRAY_BUFFER, tcbo_);
  glBufferData(GL_ARRAY_BUFFER, texture_coords_.size() * sizeof(float),
               texture_coords_.data(), GL_STATIC_DRAW);

  // Add the texture coordinates.
  glVertexAttribPointer(/*index=*/1,
                        /*size=*/2,
                        /*type=*/GL_FLOAT,
                        /*normalized=*/GL_FALSE,
                        /*stride=*/2 * sizeof(float),
                        /*offset=*/(void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind to clean up.
  // Note: The vertex array MUST be unbound first.
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Declare the texture.
  glGenTextures(1, &tile_texture_);
  glBindTexture(GL_TEXTURE_2D, tile_texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Generate the image.
  for (int i = 0; i < 64; ++i) {
    unsigned char color = 0xFF;  // white

    std::unordered_set<int> question_mark{13, 29, 38, 44, 45};
    if (question_mark.count(i) != 0) {
      color = 0x00;  // black
    }
    tile_.insert(tile_.end(), {color, color, color, 0xFF});
  }

  // Load the texture with the image.
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               tile_.data());
  glGenerateMipmap(GL_TEXTURE_2D);
}
