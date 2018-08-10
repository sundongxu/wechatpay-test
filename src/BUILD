package(default_visibility = ["//visibility:public"])

# proto rules
load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cc_proto_library")

cc_proto_library(
    name = "demo_proto",
    protos = ["demo.proto"],
    with_grpc = True,
)

cc_binary(
    name = "wechatpay_secure_server",
    srcs = ["wechatpay_secure_server.cc"],
    deps = [
        ":demo_proto",
    ],
)

cc_binary(
    name = "wechatpay_secure_client",
    srcs = ["wechatpay_secure_client.cc"],
    deps = [
        ":demo_proto",
    ],
)

