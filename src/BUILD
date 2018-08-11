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

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    deps = [
        ":demo_proto",
        ":hiredis",
        ":bcrypt",
    ],
   copts = [
       "-Isrc/include",
   ],
   linkstatic = 1,
)

cc_binary(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        ":demo_proto",
    ],
    copts = [
        "-Isrc/include",
    ],
    linkstatic = 1,
)

