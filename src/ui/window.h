#ifndef UI_WINDOW_H_
#define UI_WINDOW_H_

#include <cstdint>
#include <string>
#include <vector>

class Window {
 public:
  virtual ~Window() {}
  virtual bool IsClosed() = 0;
  virtual void SetTitle(std::string title) = 0;
  virtual void SetScroll(int x, int y) = 0;
  virtual void SetNametable(int num, std::vector<uint8_t>& nametable) = 0;
  virtual void SetAttributeTable(int num, std::vector<uint8_t>& table) = 0;
  virtual void SetFramePalette(std::vector<uint8_t>& palette) = 0;
  virtual void SetPalette(std::vector<uint8_t>& palette) = 0;
  virtual void Update() = 0;
};

class FakeWindow : public Window {
 public:
  // Window implementation:
  bool IsClosed() override { return false; }
  void SetTitle(std::string title) override {}
  void SetScroll(int x, int y) override {}
  void SetNametable(int num, std::vector<uint8_t>& nametable) override {}
  void SetAttributeTable(int num, std::vector<uint8_t>& table) override {}
  void SetFramePalette(std::vector<uint8_t>& palette) override {}
  void SetPalette(std::vector<uint8_t>& palette) override {}
  void Update() override {}
};

#endif  // UI_WINDOW_H_
