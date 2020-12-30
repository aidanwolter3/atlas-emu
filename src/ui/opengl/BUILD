cc_library(
    name = "opengl",
    visibility = ["//visibility:public"],
    hdrs = [
        "renderer.h",
        "window.h",
    ],
    srcs = [
        "gl_init.cc",
        "gl_init.h",
        "program.cc",
        "program.h",
        "renderer.cc",
        "shader.cc",
        "shader.h",
        "window.cc",
    ],
    deps = [
        "@glad",
        "@glfw",
        "@glm",
        "//src/public",
        "//src/ui",
    ],
    data = [
       "fragment.glsl",
       "vertex.glsl",
    ],
    # Suppress Apple's annoying deprecation warnings.
    # OpenGL is not going away anytime soon.
    defines = ["GL_SILENCE_DEPRECATION"],
)

cc_binary(
    name = "test",
    srcs = ["test.cc"],
    deps = [":opengl"],
)
