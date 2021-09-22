load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "route_partition_lib",
    srcs = glob([
        "*.cpp",
    ]),
    hdrs = glob([
        "*.h",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "//interface:interface",
        "//lib/config:config_lib",
        "@com_github_google_glog//:glog",
    ],
)