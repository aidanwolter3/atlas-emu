#ifndef UI_RENDERER_H_
#define UI_RENDERER_H_

#include <cstdint>
#include <vector>

class Renderer {
 public:
  virtual ~Renderer() {}
  virtual void Render(std::vector<uint8_t>& frame) = 0;
};

class FakeRenderer : public Renderer {
 public:
  // Renderer implementation:
  void Render(std::vector<uint8_t>& frame) override {}
};

#endif  // UI_RENDERER_H_
