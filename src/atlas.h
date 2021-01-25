#ifndef ATLAS_H_
#define ATLAS_H_

#include <memory>
#include <string>

#include "src/engine/engine.h"
#include "src/input/input.h"
#include "src/platform/posix.h"
#include "src/ui/renderer.h"
#include "src/ui/window.h"

class Atlas {
 public:
  Atlas(const std::string rom_file, bool headless = false);

  // Run until an error occurs, and returns true for success, or false for
  // error.
  bool Run();

  // Reset the state, so that it can be run again from the start as if it was
  // powered off, then on again.
  void Reset();

 private:
  PlatformPosix platform_;
  // Note 1: The window must be created before the renderer, so that the context
  // is available.
  //
  // Note 2: Even though these members do not have a transfer of ownership,
  // We must wrap them in unique_ptr, so that we can dynamically choose which
  // derived class to use.
  std::unique_ptr<Window> window_;
  std::unique_ptr<Renderer> renderer_;
  std::unique_ptr<Input> input_;
  std::unique_ptr<Engine> engine_;
};

#endif  // ATLAS_H_
