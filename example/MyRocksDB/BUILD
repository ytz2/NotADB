load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "rocksdb_example",
    srcs = ["main.cpp"],
    deps = [
        "//lib/engine:engine_lib",
        "@com_github_google_glog//:glog",
    ],
)