load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "mock_tcp_lib",
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
        "@com_github_jsoncpp//:jsoncpp",
        "@boost//:asio_ssl",
        "@com_github_google_glog//:glog",
    ],
)