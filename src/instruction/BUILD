cc_library(
    name = "instruction",
    visibility = ["//visibility:public"],
    hdrs = [
        "instruction.h",
        "load.h",
        "status.h",
        "store.h",
    ],
    srcs = [
        "instruction.cc",
        "load.cc",
        "status.cc",
        "store.cc",
    ],
    deps = [
        "//src:public",
    ]
)

cc_test(
    name = "instruction_test",
    srcs = [
        "load_test.cc",
        "status_test.cc",
    ],
    deps = [
        ":instruction",
        "@gtest//:gtest_main",
    ]
)
