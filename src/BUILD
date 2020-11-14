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
        "//src/instruction",
        "//src/public",
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
        "//src/roms:pretty_log_test",
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
