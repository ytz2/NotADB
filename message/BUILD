load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "message",
    srcs = glob([
        "commonjson/*.cpp",
        "MessageFacotry.cpp",
    ]),
    hdrs = glob([
        "commonjson/*.h"
    ]) + glob([
        "*.h"
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "//interface:interface",
        "@com_github_jsoncpp//:jsoncpp",
    ],
)