from conans import ConanFile, CMake


class Polynomial(ConanFile):
    name = "Polynomial"
    version = "1.0.1"
    author = "Baranov Konstantin (seigtm@gmail.com)"
    url = "https://github.com/seigtm/Polynomial"
    description = "A simple polynomial class that supports all basic operations"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    build_requires = \
        "gtest/[~1.11]"

    scm = {
        "type": "git",
        "subfolder": name,
        "url": "auto",
        "revision": "auto",
        "username": "git"
    }

    def _configure(self, verbose=True):
        cmake = CMake(self)
        cmake.verbose = verbose
        cmake.definitions['CMAKE_BUILD_TYPE'] = "Debug" if self.settings.build_type == "Debug" else "Release"
        cmake.configure(source_folder=self.name)
        return cmake

    def build(self):
        cmake = self._configure()
        cmake.build()
        cmake.test(output_on_failure=True)

    def package(self):
        cmake = self._configure()
        cmake.install()