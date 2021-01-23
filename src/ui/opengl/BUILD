cc_library(
    name = "opengl",
    visibility = ["//src:__pkg__"],
    hdrs = [
        "renderer.h",
        "renderer2.h",
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
        "renderer2.cc",
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
        "//src/ui",
    ],
    data = [
       "background.frag",
       "background.vert",
       "renderer2.frag",
       "renderer2.vert",
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

cc_binary(
    name = "test2",
    srcs = ["test2.cc"],
    deps = [":opengl"],
)
