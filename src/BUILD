cc_library(
    name = "atlas_core",
    hdrs = [
        "atlas.h",
        "bus_impl.h",
        "cpu.h",
        "event_logger_impl.h",
        "memory.h",
        "mmc1.h",
        "oamdma.h",
        "ppu.h",
    ],
    srcs = [
        "atlas.cc",
        "bus_impl.cc",
        "cpu.cc",
        "event_logger_impl.cc",
        "memory.cc",
        "mmc1.cc",
        "oamdma.cc",
        "ppu.cc",
    ],
    deps = [
        "//src/instruction",
        "//src/platform",
        "//src/public",
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
    srcs = [
        "atlas_test.cc",
        "bus_impl_test.cc",
        "cpu_test.cc",
        "memory_test.cc",
        "mmc1_test.cc",
    ],
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
