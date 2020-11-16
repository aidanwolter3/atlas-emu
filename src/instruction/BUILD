cc_library(
    name = "instruction",
    visibility = ["//visibility:public"],
    hdrs = [
        "branch.h",
        "load.h",
        "logic.h",
        "misc.h",
        "shift.h",
        "stack.h",
        "status.h",
        "store.h",
    ],
    srcs = [
        "branch.cc",
        "instruction.cc",
        "load.cc",
        "logic.cc",
        "misc.cc",
        "shift.cc",
        "stack.cc",
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
        "stack_test.cc",
        "status_test.cc",
        "store_test.cc",
    ],
    deps = [
        ":instruction",
        "@gtest//:gtest_main",
    ]
)
