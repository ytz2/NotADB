cc_library(
    name = "jsoncpp",
    srcs = glob(["src/lib_json/*.cpp"]),
    hdrs = glob(["src/lib_json/*.h", "src/lib_json/*.inl", "include/**/*.h"]),
    includes = ["include"],
    copts = [
            "-std=c++17",
    ],
    visibility = ["//visibility:public"],
)
