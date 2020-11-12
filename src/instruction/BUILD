cc_library(
    name = "instruction",
    visibility = ["//visibility:public"],
    hdrs = [
        "load.h",
        "misc.h",
        "status.h",
        "store.h",
    ],
    srcs = [
        "instruction.cc",
        "load.cc",
        "misc.cc",
        "status.cc",
        "store.cc",
    ],
    deps = [
        "//src/public",
    ]
)

cc_test(
    name = "instruction_test",
    srcs = [
        "instruction_test_base.cc",
        "instruction_test_base.h",
        "load_test.cc",
        "misc_test.cc",
        "status_test.cc",
        "store_test.cc",
    ],
    deps = [
        ":instruction",
        "@gtest//:gtest_main",
    ]
)
