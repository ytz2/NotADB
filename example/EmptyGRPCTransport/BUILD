load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "empty_grpd_transport",
    srcs = ["main.cpp"],
    deps = [
        "//lib/grpc:grpc_lib",
        "@com_github_google_glog//:glog",
    ],
)