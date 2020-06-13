package(default_visibility = ["//visibility:public"],)

filegroup(
    name = "ld_data",
    srcs = glob([
        "cfg/*.cfg",
        "lib/*.lib",
    ]),
)

cc_binary(
    name = "ca65",
    srcs = glob(["src/ca65/*", "src/common/*"]),
    includes = ["src/common"],
)

cc_binary(
    name = "ld65",
    srcs = glob(["src/ld65/*", "src/common/*"]),
    includes = ["src/common"],
    data = [":ld_data"],
)
