load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "config_lib",
    srcs = glob([
        "*.cpp",
    ]),
    hdrs = glob([
        "*.h",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_jbeder_yaml_cpp//:yaml-cpp",
        "@com_github_google_glog//:glog",
        "@boost//:algorithm",
    ],
)