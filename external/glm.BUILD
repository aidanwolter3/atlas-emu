package(default_visibility = ["//visibility:public"],)

load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

filegroup(
    name = "glm_files",
    srcs = glob(["**"]),
)

cc_library(
    name = "glm_config",
    includes = [""],
    srcs = glob([
        "**/*.hpp",
        "**/*.h",
        "**/*.inl",
    ]),
)

cmake_external(
    name = "glm",
    lib_source = ":glm_files",
    static_libraries = ["libglm_static.a"],
    out_lib_dir = "",
    make_commands = ["make"],
    cache_entries = {
        "GLM_TEST_ENABLE": "OFF",
        "BUILD_STATIC_LIBS": "ON"
    },
    deps = [":glm_config"],
)
