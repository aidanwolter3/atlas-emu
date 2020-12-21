#ifndef UI_WINDOW_H_
#define UI_WINDOW_H_

#include <unistd.h>

#include <vector>

class Window {
 public:
  virtual bool IsClosed() = 0;
  virtual void SetTile(int num, std::vector<uint8_t>& tile) = 0;
  virtual void Update() = 0;
};

#endif  // UI_WINDOW_H_
