package(default_visibility = ["//visibility:public"],)

load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

filegroup(
    name = "glad_files",
    srcs = glob(["**"]),
)

cmake_external(
    name = "glad",
    lib_source = ":glad_files",
    static_libraries = ["libglad.a"],
    cache_entries = {
        "GLAD_REPRODUCIBLE": "ON",
        "GLAD_INSTALL": "ON",
    },
)
