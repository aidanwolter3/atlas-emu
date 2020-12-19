package(default_visibility = ["//visibility:public"],)

load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

filegroup(
    name = "glfw_files",
    srcs = glob(["**"]),
)

# TODO: Swap these out with a config_setting so that it works on both Mac and
# Linux.
cc_library(
    name = "glfw_config",
    linkopts = [
      "-framework CoreVideo",
      "-framework OpenGL",
      "-framework IOKit",
      "-framework Cocoa",
      "-framework Carbon",
    ]
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
    deps = ["glfw_config"],
)
