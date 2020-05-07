cc_library(
    name = "atlas",
    hdrs = [
        "atlas.h",
        "cpu.h",
        "memory.h",
    ],
    srcs = [
        "atlas.cc",
        "cpu.cc",
        "memory_impl.cc",
        "memory_impl.h",
    ]
)

cc_test(
    name = "atlas_test",
    srcs = [
        "atlas_test.cc",
        "cpu_test.cc",
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
