cc_library(
    name = "engine_core",
    hdrs = [
        "bus_impl.h",
        "cpu.h",
        "joystick.h",
        "memory.h",
        "mmc1.h",
        "oamdma.h",
        "ppu.h",
    ],
    srcs = [
        "bus_impl.cc",
        "cpu.cc",
        "joystick.cc",
        "memory.cc",
        "mmc1.cc",
        "oamdma.cc",
        "ppu.cc",
    ],
    deps = [
        "//src/engine/base",
        "//src/engine/instruction",
        "//src/input",
        "//src/ui",
    ],
)

cc_library(
    name = "engine",
    visibility = ["//src:__pkg__"],
    hdrs = ["engine.h"],
    srcs = ["engine.cc"],
    deps = [
        ":engine_core",
        "//src/engine/base",
        "//src/ui",
    ],
)

cc_test(
    name = "engine_test",
    srcs = [
        "bus_impl_test.cc",
        "cpu_test.cc",
        "memory_test.cc",
        "mmc1_test.cc",
    ],
    deps = [
        ":engine_core",
        "@gtest//:gtest_main",
    ],
)
