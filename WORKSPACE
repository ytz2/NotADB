load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

http_archive(
    name = "com_github_jsoncpp",
    url = "https://github.com/open-source-parsers/jsoncpp/archive/1.8.4.zip",
    build_file = "//third_party:jsoncpp.BUILD",
    strip_prefix = "jsoncpp-1.8.4",
)

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "1e3a69bf2d5cd10c34b74f066054cd335d033d71",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1591047380 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

http_archive(
    name = "com_github_google_glog",
    sha256 = "21bc744fb7f2fa701ee8db339ded7dce4f975d0d55837a97be7d46e8382dea5a",
    strip_prefix = "glog-0.5.0",
    urls = ["https://github.com/google/glog/archive/v0.5.0.zip"],
)


http_archive(
    name = "com_github_jbeder_yaml_cpp",
    sha256 = "1df36d25ff796a79bee69ea42ee2b86be09ac9f3582efaf5a26037f09033297b",
    strip_prefix = "yaml-cpp-yaml-cpp-0.6.4",
    urls = [
        "https://github.com/sammi/yaml-cpp/archive/yaml-cpp-0.6.4.tar.gz",
    ],
)

git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    branch = "v1.10.x",
)

new_git_repository(
    name = "com_github_edenhill_librdkafka",
    remote = "https://github.com/edenhill/librdkafka.git",
    tag = "v1.7.0",
    build_file = "//third_party:librdkafka.BUILD",
)

bind(
    name = "kafka",
    actual = "@com_github_edenhill_librdkafka//:kafka",
)

bind(
    name = "kafkacpp",
    actual = "@com_github_edenhill_librdkafka//:kafkacpp",
)

new_git_repository(
    name = "com_github_morganstanley_moderncppkafka",
    remote = "https://github.com/morganstanley/modern-cpp-kafka.git",
    commit = "82bdae9a29fda7bac94a40c80cc2e6ff3bda51bd",
    build_file = "//third_party:moderncppkafka.BUILD",
    shallow_since = "1631534515 +0800",
)