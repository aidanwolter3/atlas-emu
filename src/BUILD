cc_library(
    name = "public",
    visibility = ["//visibility:public"],
    hdrs = [
        "memory.h",
        "registers.h",
    ]
)

cc_library(
    name = "atlas",
    hdrs = [
        "atlas.h",
        "cpu.h",
        "memory_impl.h",
    ],
    srcs = [
        "atlas.cc",
        "cpu.cc",
        "memory_impl.cc",
    ],
    deps = [
        ":public",
        "//src/instruction",
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
    data = [
        "//src/roms:nop_test",
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
