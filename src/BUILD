package(default_visibility = ["//visibility:public"])

# proto rules
load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cc_proto_library")

cc_proto_library(
    name = "demo_proto",
    protos = ["demo.proto"],
    with_grpc = True,
)

cc_import(
    name = "hiredis",
    static_library = "libhiredis.a",
)

cc_import(
    name = "bcrypt",
    static_library = "libbcrypt.a",
)

cc_library(
    name = "comm",
    hdrs = [
        "include/comm_def.h",
    ],
)

cc_library(
    name = "util",
    hdrs = [
        "include/util.h",
    ],
)

cc_library(
    name = "hiredis_handler",
    hdrs = glob([
        "include/hiredis/*.h",
    ]),
    deps = [
        ":hiredis",
    ],
)

cc_library(
    name = "redis_handler",
    hdrs = [
        "include/redis_handler.h",
    ],
    srcs = [
        "redis_handler.cc",
    ],
    deps = [
        ":hiredis_handler",
    ],
)

cc_library(
    name = "bcrypt_handler",
    hdrs = glob([
        "include/bcrypt/*.hpp",
        "include/bcrypt/*.h",
    ]),
    deps = [
        ":bcrypt",
    ],
)

cc_library(
    name = "entity",
    hdrs = [
        "include/entity.h",
    ],
    srcs = [
        "wcp_client.cc",
        "wcp_server.cc",
    ],
    deps = [
        ":comm",
        ":util",
        ":redis_handler",
        ":bcrypt_handler",
        ":demo_proto",
    ],
)

cc_binary(
    name = "client",
    srcs = [
        "client.cc",
        ],
    deps = [
        ":entity",
    ],
)

cc_binary(
    name = "server",
    srcs = [
        "server.cc",
        ],
    deps = [
        ":entity",
    ],
)


