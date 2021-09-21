from conans import ConanFile, CMake


class Polynomial(ConanFile):
    name = "Polynomial"
    version = "1.0.4"

    # Metadata.
    author = "Baranov Konstantin (seigtm@gmail.com)"
    url = "https://github.com/seigtm/Polynomial"
    description = "A simple polynomial class that supports all basic operations"

    # Binary configuration.
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    # Packages requirements.
    requires = \
        "boost/[1.77.0]"

    build_requires = \
        "gtest/[1.11]"

    # Version control specification.
    scm = {
        "type": "git",
        "subfolder": name,
        "url": "auto",
        "revision": "auto",
        "username": "git"
    }

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder = self.name)
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.configure(source_folder = self.name)
        cmake.install()
