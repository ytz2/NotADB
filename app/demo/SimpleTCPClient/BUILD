load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "simple_tcp_client",
    srcs = ["main.cpp"],
    deps = [
        "//lib/tcpconnection:tcp_lib",
        "@com_github_google_glog//:glog",
    ],
)