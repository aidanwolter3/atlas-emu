#ifndef UI_RENDERER2_H_
#define UI_RENDERER2_H_

#include <cstdint>
#include <vector>

class Renderer2 {
 public:
  virtual ~Renderer2() {}
  virtual void Render(std::vector<uint8_t>& frame) = 0;
};

class FakeRenderer2 : public Renderer2 {
 public:
  // Renderer implementation:
  void Render(std::vector<uint8_t>& frame) override {}
};

#endif  // UI_RENDERER2_H_
