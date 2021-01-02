#ifndef ATLAS_H_
#define ATLAS_H_

#include <string>

#include "src/engine/engine.h"
#include "src/platform/posix.h"
#include "src/ui/opengl/renderer.h"
#include "src/ui/opengl/window.h"

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
  // The window must be created before the renderer, so that the context is
  // available.
  OpenGLWindow window_;
  OpenGLRenderer renderer_;
  PlatformPosix platform_;
  Engine engine_;
};

#endif  // ATLAS_H_
