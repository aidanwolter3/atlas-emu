package(default_visibility = ["//visibility:public"],)

load("@rules_foreign_cc//foreign_cc:cmake.bzl", "cmake")

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

cmake(
    name = "glm",
    lib_source = ":glm_files",
    out_static_libs = ["libglm_static.a"],
    out_lib_dir = "",
    install = False,
    cache_entries = {
        "GLM_TEST_ENABLE": "OFF",
        "BUILD_STATIC_LIBS": "ON"
    },
    deps = [":glm_config"],
)
