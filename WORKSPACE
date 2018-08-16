# protobuf & grpc rules
# Enable C++ support by loading the dependencies
# bazel project
git_repository(
    name = "org_pubref_rules_protobuf",
    remote = "https://github.com/pubref/rules_protobuf",
    tag = "v0.8.1",
)

# non-bazel(make) project
# hiredis
new_git_repository(
    name = "myhiredis",
    remote = "https://github.com/sundongxu/hiredis.git",
    commit = "846144a05c70bd91d0fe081aae62c18132575412",
    build_file = "hiredis.BUILD",
)

# non-bazel(make) project
# libbcrypt
new_git_repository(
    name = "mybcrypt",
    remote = "https://github.com/sundongxu/libbcrypt.git",
    commit = "4c1287919082d6c6c8809c8aed944e0f246a238d",
    build_file = "libbcrypt.BUILD",
)

load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cpp_proto_repositories")
cpp_proto_repositories()
