cc_library(
    name = "opengl",
    hdrs = [
        "window.h",
    ],
    srcs = [
        "window.cc",
        "program.cc",
        "program.h",
        "shader.cc",
        "shader.h",
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
    visibility = [
        "//visibility:public",
    ],
)
