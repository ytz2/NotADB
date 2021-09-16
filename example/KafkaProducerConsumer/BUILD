load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "kafka_producer_consumer",
    srcs = ["main.cpp"],
    linkopts = ['"-L/usr/local/Cellar/lz4/1.9.3/lib"', '"-llz4"', '"-lm"', '"-L/usr/local/Cellar/zstd/1.5.0/lib"', '"-lzstd"', '"-lsasl2"', '"-L/usr/local/Cellar/openssl@1.1/1.1.1l/lib"', '"-lssl"', '"-L/usr/local/Cellar/openssl@1.1/1.1.1l/lib"', '"-lcrypto"', '"-lz"', '"-ldl"', '"-lpthread"'],
    deps = [
        "//lib/kafka:kafka_lib",
        "//lib/config:config_lib",
        "@com_github_google_glog//:glog",
    ],
)