load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Google testing framework version 1.10.0.
git_repository(
    name = "gtest",
    commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
    shallow_since = "1570114335 -0400",
    remote = "https://github.com/google/googletest.git",
)

# Benchmarking framework version 1.5.2.
git_repository(
    name = "benchmark",
    commit = "73d4d5e8d6d449fc8663765a42aa8aeeee844489",
    shallow_since = "1599818118 +0100",
    remote = "https://github.com/google/benchmark.git",
)

# Compiler/Assembler for 65XX architectures.
http_archive(
    name = "cc65",
    urls = ["https://github.com/cc65/cc65/archive/V2.18.zip"],
    sha256 = "aad4dbcb3152d1ad20d68083048159538cc50234097a09ee079e801a3a8bea2f",
    strip_prefix = "cc65-2.18",
    build_file = "cc65.BUILD",
)

# Rule repository for building CMake dependencies.
http_archive(
   name = "rules_foreign_cc",
   urls = ["https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip"],
   sha256 = "3e6b0691fc57db8217d535393dcc2cf7c1d39fc87e9adb6e7d7bab1483915110",
   strip_prefix = "rules_foreign_cc-master",
)

# Initialize the CMake dependencies.
load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies([])

# GL Framework - Window management
http_archive(
    name = "glfw",
    urls = ["https://github.com/glfw/glfw/archive/3.3.2.zip"],
    sha256 = "33c6bfc422ca7002befbb39a7a60dc101d32c930ee58be532ffbcd52e9635812",
    strip_prefix = "glfw-3.3.2",
    build_file = "glfw.BUILD",
)

# GL Loader
http_archive(
    name = "glad",
    urls = ["https://github.com/Dav1dde/glad/archive/v0.1.34.zip"],
    sha256 = "19b8a8b10deef3de4b31d3bde488d28d319db6968fd128be43c8040aef2a0622",
    strip_prefix = "glad-0.1.34",
    build_file = "glad.BUILD",
)

# GL Mathmatics - Matrices and stuff
http_archive(
    name = "glm",
    urls = ["https://github.com/g-truc/glm/archive/0.9.9.7.zip"],
    sha256 = "c5e167c042afd2d7ad642ace6b643863baeb33880781983563e1ab68a30d3e95",
    strip_prefix = "glm-0.9.9.7",
    build_file = "glm.BUILD",
)
