cc_library(
    name = "atlas",
    hdrs = [
        "atlas.h",
        "memory.h",
    ],
    srcs = [
        "atlas.cc",
        "memory_impl.cc",
        "memory_impl.h",
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
