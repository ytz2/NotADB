load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "simple_rocksdb_example",
    srcs = ["simple_rocksdb.cpp"],
    deps = [
        "//lib/engine:engine_lib",
        "@com_github_google_glog//:glog",
    ],
)


cc_binary(
    name = "replicable_rocksdb_example",
    srcs = ["replicable_rocksdb.cpp"],
    deps = [
        "//lib/engine:engine_lib",
        "@com_github_google_glog//:glog",
    ],
)