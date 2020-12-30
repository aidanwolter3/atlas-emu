#ifndef UI_WINDOW_H_
#define UI_WINDOW_H_

#include <string>

class Window {
 public:
  virtual ~Window() {}
  virtual void Refresh() = 0;
  virtual bool IsClosed() = 0;
  virtual void SetTitle(std::string title) = 0;
};

class FakeWindow : public Window {
 public:
  // Window implementation:
  void Refresh() override {}
  bool IsClosed() override { return false; }
  void SetTitle(std::string title) override {}
};

#endif  // UI_WINDOW_H_
