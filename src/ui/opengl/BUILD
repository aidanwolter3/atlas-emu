cc_library(
    name = "opengl",
    visibility = ["//visibility:public"],
    hdrs = [
        "renderer.h",
        "window.h",
    ],
    srcs = [
        "background.cc",
        "background.h",
        "gl_init.cc",
        "gl_init.h",
        "program.cc",
        "program.h",
        "renderer.cc",
        "shader.cc",
        "shader.h",
        "sprites.cc",
        "sprites.h",
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
       "background.frag",
       "background.vert",
       "sprites.frag",
       "sprites.vert",
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
