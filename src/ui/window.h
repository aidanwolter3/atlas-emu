#ifndef UI_WINDOW_H_
#define UI_WINDOW_H_

#include <unistd.h>

#include <vector>

class Window {
 public:
  virtual ~Window() {}
  virtual bool IsClosed() = 0;
  virtual void SetTile(int num, std::vector<uint8_t>& tile) = 0;
  virtual void Update() = 0;
};

class FakeWindow : public Window {
 public:
  // Window implementation:
  bool IsClosed() override { return false; }
  void SetTile(int num, std::vector<uint8_t>& tile) override {}
  void Update() override {}
};

#endif  // UI_WINDOW_H_
