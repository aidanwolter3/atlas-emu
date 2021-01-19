cc_library(
    name = "instruction",
    visibility = ["//src/engine:__pkg__"],
    hdrs = [
        "addressing.h",
        "branch.h",
        "compare.h",
        "instruction.h",
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
        "addressing.cc",
        "branch.cc",
        "compare.cc",
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
        "//src/engine/base",
    ]
)

cc_test(
    name = "instruction_test",
    srcs = [
        "branch_test.cc",
        "compare_test.cc",
        "instruction_test_base.h",
        "jump_test.cc",
        "load_test.cc",
        "logic_test.cc",
        "math_test.cc",
        "misc_test.cc",
        "shift_test.cc",
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
