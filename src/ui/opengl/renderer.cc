#include "src/ui/opengl/renderer.h"

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include "src/ui/opengl/gl_init.h"
#include "src/ui/opengl/shader.h"

OpenGLRenderer::OpenGLRenderer() {
  InitGLIfNeeded();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  std::vector<std::unique_ptr<Shader>> shaders;
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_VERTEX_SHADER, "src/ui/opengl/Renderer.vert")));
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_FRAGMENT_SHADER, "src/ui/opengl/Renderer.frag")));
  program_ = std::make_unique<Program>(std::move(shaders));

  // Prepare the texture.
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &frame_);
  glBindTexture(GL_TEXTURE_2D, frame_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Prepare the elements.
  // TODO: remove the Z coordinate after switching to Renderer.
  program_->SetVertices({
      -1.0, 1.0, 0.0,   // top-left
      1.0, 1.0, 0.0,    // top-right
      1.0, -1.0, 0.0,   // bottom-right
      -1.0, -1.0, 0.0,  // bottom-left
  });
  program_->SetElements({
      0,
      1,
      2,
      2,
      3,
      0,
  });
  // TODO: remove the Z coordinate after switching to Renderer.
  program_->SetTextureCoords({
      0.0, 0.0, 0.0,  // bottom-left
      1.0, 0.0, 0.0,  // bottom-right
      1.0, 1.0, 0.0,  // top-right
      0.0, 1.0, 0.0,  // top-left
  });
}

OpenGLRenderer::~OpenGLRenderer() = default;

void OpenGLRenderer::Render(std::vector<uint8_t>& frame) {
  if (frame.size() != (32 * 30 * 64 * 3)) {
    std::cout << "Frame is incorrect size" << std::endl;
    return;
  }

  glClear(GL_COLOR_BUFFER_BIT);
  program_->Use();

  // Set the frame data.
  glUniform1i(program_->GetUniformLocation("frame"), 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, frame_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               /*width=*/32 * 8,
               /*height=*/30 * 8, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data());

  program_->Draw();
}
