cc_library(
    name = "base",
    hdrs = [
        "program.h",
        "shader.h",
        "window.h",
    ],
    srcs = [
        "program.cc",
        "shader.cc",
        "window.cc",
    ],
    deps = [
        "@glad",
        "@glfw",
        "@glm",
    ],
)

cc_binary(
    name = "test",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":base",
    ],
    data = [
       "fragment.glsl",
       "vertex.glsl",
    ]
)
