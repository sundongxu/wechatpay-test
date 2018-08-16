cc_library(
    name = "libbcrypt",
    srcs = glob([
        "src/*.c",
        "src/x86.S"
    ]),
    hdrs = glob([
        "include/bcrypt/*.h",
        "include/bcrypt/*.hpp"
    ]),
    copts = ["-Iexternal/libbcrypt/include"],
    visibility = ["//visibility:public"],
)