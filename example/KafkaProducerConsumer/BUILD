load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "kafka_producer_consumer",
    srcs = ["main.cpp"],
    deps = [
        "//lib/kafka:kafka_lib",
        "//lib/config:config_lib",
        "@com_github_google_glog//:glog",
    ],
)