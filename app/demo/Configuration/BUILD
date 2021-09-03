load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "demo_config",
    srcs = ["main.cpp"],
    deps = [
        "//lib/config:config_lib",
        "@com_github_google_glog//:glog",
    ],
)