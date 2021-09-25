cc_test (
    name = "route_test",
    srcs = [
        "Route_test.cpp",
    ],
    deps = [
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@com_github_google_glog//:glog",
         "//lib/route:route_lib",
    ],
    copts = [
        "-Wno-macro-redefined",
    ],
)