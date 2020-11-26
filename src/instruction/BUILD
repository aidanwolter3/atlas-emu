cc_library(
    name = "instruction",
    visibility = ["//visibility:public"],
    hdrs = [
        "branch.h",
        "compare.h",
        "jump.h",
        "load.h",
        "logic.h",
        "math.h",
        "misc.h",
        "shift.h",
        "stack.h",
        "status.h",
        "store.h",
        "transfer.h",
    ],
    srcs = [
        "branch.cc",
        "compare.cc",
        "instruction.cc",
        "jump.cc",
        "load.cc",
        "logic.cc",
        "math.cc",
        "misc.cc",
        "shift.cc",
        "stack.cc",
        "status.cc",
        "store.cc",
        "transfer.cc",
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
        "math_test.cc",
        "misc_test.cc",
        "stack_test.cc",
        "status_test.cc",
        "store_test.cc",
        "transfer_test.cc",
    ],
    deps = [
        ":instruction",
        "@gtest//:gtest_main",
    ]
)
