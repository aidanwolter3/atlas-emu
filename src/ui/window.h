#ifndef UI_WINDOW_H_
#define UI_WINDOW_H_

#include <cstdint>
#include <vector>

class Window {
 public:
  virtual ~Window() {}
  virtual bool IsClosed() = 0;
  virtual void SetTile(int num, std::vector<uint8_t>& tile) = 0;
  virtual void SetAttributeTable(int num, std::vector<uint8_t>& table) = 0;
  virtual void SetFramePalette(std::vector<uint8_t>& palette) = 0;
  virtual void SetPalette(std::vector<uint8_t>& palette) = 0;
  virtual void Update() = 0;
};

class FakeWindow : public Window {
 public:
  // Window implementation:
  bool IsClosed() override { return false; }
  void SetTile(int num, std::vector<uint8_t>& tile) override {}
  void SetAttributeTable(int num, std::vector<uint8_t>& table) override {}
  void SetFramePalette(std::vector<uint8_t>& palette) override {}
  void SetPalette(std::vector<uint8_t>& palette) override {}
  void Update() override {}
};

#endif  // UI_WINDOW_H_
