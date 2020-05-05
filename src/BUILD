cc_library(
    name = "atlas_core",
    srcs = [
        "atlas.cc",
        "atlas.h",
    ]
)

cc_binary(
    name = "atlas",
    visibility = [
        "//visibility:public",
    ],
    srcs = [
        "main.cc",
    ],
    deps = [
        ":atlas_core",
    ]
)
