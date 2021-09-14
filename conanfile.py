from conans import ConanFile, CMake


class Polynomial(ConanFile):
    name = "Polynomial"
    version = "1.0.3"
    author = "Baranov Konstantin (seigtm@gmail.com)"
    url = "https://github.com/seigtm/Polynomial"
    description = "A simple polynomial class that supports all basic operations"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    requires = \
        "boost/[1.77.0]"

    build_requires = \
        "gtest/[1.11]"
