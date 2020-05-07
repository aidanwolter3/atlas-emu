cc_library(
    name = "atlas",
    hdrs = [
        "atlas.h",
    ],
    srcs = [
        "atlas.cc",
        "memory.cc",
        "memory.h",
    ]
)

cc_test(
    name = "atlas_test",
    srcs = [
        "atlas_test.cc",
        "memory_test.cc",
    ],
    deps = [
        ":atlas",
        "@gtest//:gtest_main",
    ],
)

cc_binary(
    name = "atlas_emu",
    visibility = [
        "//visibility:public",
    ],
    srcs = [
        "main.cc",
    ],
    deps = [
        ":atlas",
    ]
)
