load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "engine_lib",
    srcs = glob([
        "*.cpp",
    ]),
    hdrs = glob([
        "*.h",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "//interface:interface",
        "//message:message",
        "//lib/config:config_lib",
        "//lib/kafka:kafka_lib",
        "@boost//:lexical_cast",
        "@com_github_google_glog//:glog",
        "@com_github_facebook_rocksdb//:rocksdb",
    ],
)