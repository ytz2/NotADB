load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "mock_server_client",
    srcs = ["main.cpp"],
    deps = [
        "//lib/mockconnection:mock_tcp_lib",
        "@com_github_google_glog//:glog",
    ],
)