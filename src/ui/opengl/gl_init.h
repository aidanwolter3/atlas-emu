#ifndef UI_OPENGL_GL_INIT_H_
#define UI_OPENGL_GL_INIT_H_

// Ensures that GL is initialized.
// This uses TLS to ensure that only the first call affects anything.
void InitGLIfNeeded();

#endif  // UI_OPENGL_GL_INIT_H_
