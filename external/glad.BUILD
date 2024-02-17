package(default_visibility = ["//visibility:public"],)

load("@rules_foreign_cc//foreign_cc:cmake.bzl", "cmake")

filegroup(
    name = "glad_files",
    srcs = glob(["**"]),
)

cmake(
    name = "glad",
    lib_source = ":glad_files",
    out_static_libs = ["libglad.a"],
    cache_entries = {
        "GLAD_REPRODUCIBLE": "ON",
        "GLAD_INSTALL": "ON",
    },
)
