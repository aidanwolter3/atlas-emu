load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Google testing framework version 1.10.0.
git_repository(
    name = "gtest",
    commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
    shallow_since = "1570114335 -0400",
    remote = "https://github.com/google/googletest.git",
)

# Compiler/Assembler for 65XX architectures.
http_archive(
    name = "cc65",
    urls = [ "https://github.com/cc65/cc65/archive/V2.18.zip"],
    sha256 = "aad4dbcb3152d1ad20d68083048159538cc50234097a09ee079e801a3a8bea2f",
    strip_prefix = "cc65-2.18",
    build_file = "cc65.BUILD",
)
