cc_library(
    name = "atlas_core",
    hdrs = ["atlas.h"],
    srcs = ["atlas.cc"],
    deps = [
        "//src/engine",
        "//src/platform",
        "//src/ui/opengl",
    ],
)

cc_binary(
    name = "atlas",
    visibility = ["//visibility:public"],
    srcs = ["main.cc"],
    deps = [":atlas_core"],
)

cc_test(
    name = "atlas_test",
    srcs = ["atlas_test.cc"],
    deps = [
        ":atlas_core",
        "@gtest//:gtest_main",
    ],
    data = [
        "//src/roms:branch_test",
        "//src/roms:pretty_log_test",
        "//src/roms:ppu_test",
    ],
    linkstatic = True,
)

cc_binary(
    name = "benchmark",
    visibility = ["//visibility:public"],
    srcs = ["benchmark.cc"],
    deps = [
        ":atlas_core",
        "@benchmark//:benchmark",
    ],
    data = [
        "//src/roms:100_nops",
    ],
)
