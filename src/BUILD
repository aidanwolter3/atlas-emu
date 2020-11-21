cc_library(
    name = "atlas",
    hdrs = [
        "atlas.h",
        "bus_impl.h",
        "cpu.h",
        "memory.h",
        "storage.h",
    ],
    srcs = [
        "atlas.cc",
        "bus_impl.cc",
        "cpu.cc",
        "memory.cc",
        "storage.cc"
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
        "bus_impl_test.cc",
        "cpu_test.cc",
        "memory_test.cc",
        "storage_test.cc",
    ],
    deps = [
        ":atlas",
        "@gtest//:gtest_main",
    ],
    data = [
        "//src/roms:branch_test",
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
