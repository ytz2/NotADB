genrule(
    name = "librdkafka-a",
    cmd = "cd external/com_github_edenhill_librdkafka && ./configure && make && mv src/librdkafka.a ../../$@",
    srcs = glob(["**"]),
    outs = ["src/librdkafka.a"],
)

genrule(
    name = "librdkafka++-a",
    cmd = "cd external/com_github_edenhill_librdkafka && ./configure && make && mv src/librdkafka++.a ../../$@",
    srcs = glob(["**"]),
    outs = ["src/librdkafka++.a"],
)

cc_import(
    name = "librdkafka",
    static_library = ":librdkafka-a",
)

cc_import(
    name = "librdkafka++",
    static_library = ":librdkafka++-a",
)

cc_library(
    name = "kafka",
    hdrs = ["src/rdkafka.h"],
    strip_include_prefix = 'src',
    include_prefix = 'librdkafka',
    deps = [
      ":librdkafka",
    ],
    visibility = ["//visibility:public"],
)


cc_library(
    name = "kafkacpp",
    hdrs = ["src-cpp/rdkafkacpp.h"],
    strip_include_prefix = 'src-cpp',
    include_prefix = 'librdkafka',
    deps = [
      ":librdkafka",
      ":kafka",
    ],
    visibility = ["//visibility:public"],
)