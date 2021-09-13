load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "kafka_lib",
    srcs = glob([
        "*.cpp",
    ]),
    hdrs = glob([
        "*.h",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_morganstanley_moderncppkafka//:modern_cpp_kafka"
    ],
)