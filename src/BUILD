cc_library(
    name = "atlas",
    hdrs = [
        "atlas.h",
    ],
    srcs = [
        "atlas.cc",
    ]
)

cc_test(
    name = "atlas_test",
    srcs = [
        "atlas_test.cc",
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
