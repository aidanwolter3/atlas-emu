package(default_visibility = ["//visibility:public"],)

load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

filegroup(
    name = "glfw_files",
    srcs = glob(["**"]),
)

cc_library(
    name = "linux_config",
    linkopts = [
      "-lX11",
      "-ldl",
      "-pthread",
    ]
)

cc_library(
    name = "osx_config",
    linkopts = [
      "-framework CoreVideo",
      "-framework OpenGL",
      "-framework IOKit",
      "-framework Cocoa",
      "-framework Carbon",
    ]
)

config_setting(
    name = "linux",
    constraint_values = [
        "@platforms//os:linux",
    ],
)

config_setting(
    name = "osx",
    constraint_values = [
        "@platforms//os:osx",
    ],
)

cmake_external(
    name = "glfw",
    lib_source = ":glfw_files",
    static_libraries = ["libglfw3.a"],
    cache_entries = {
        "GLFW_BUILD_TESTS": "OFF",
        "GLFW_BUILD_DOCS": "OFF",
        "GLFW_BUILD_EXAMPLES": "OFF",
    },
    deps = select({
        ":linux": [":linux_config"],
        ":osx": [":osx_config"],
    }),
)
