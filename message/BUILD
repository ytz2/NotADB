load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "message",
    srcs = glob([
        "avro/*.cpp",
        "commonjson/*.cpp",
        "MessageFacotry.cpp",
    ]),
    hdrs = glob([
        "avro/*.h"
     ]) +glob([
        "commonjson/*.h"
    ]) + glob([
        "*.h"
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "//interface:interface",
        "@com_github_jsoncpp//:jsoncpp",
        "@avro_archive//:avro",
    ],
)