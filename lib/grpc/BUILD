load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "grpc_lib",
    srcs = glob([
        "*.cpp",
    ]),
    hdrs = glob([
        "*.h",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "//interface:interface",
        "//rpc/HealthCheck:health_check_cc_grpc",
        "@com_github_google_glog//:glog",
        "@com_github_grpc_grpc//:grpc++",
    ],
)