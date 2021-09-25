package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

# The following three rules demonstrate the usage of the cc_grpc_library rule in
# in a mode compatible with the native proto_library and cc_proto_library rules.
proto_library(
    name = "notadb_proto",
    srcs = ["not_a_db.proto"],
)

cc_proto_library(
    name = "notadb_cc_proto",
    deps = [":notadb_proto"],
)

cc_grpc_library(
    name = "notadb_cc_grpc",
    srcs = [":notadb_proto"],
    grpc_only = True,
    deps = [":notadb_cc_proto"],
)
