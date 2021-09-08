#include <gtest/gtest.h>
#include <polynomial.hpp>
#include "polynomialTests.hpp"

using ::testing::InitGoogleTest;
using namespace setm;  // Polynomial namespace.

int main(int argc, char *argv[]) {
    InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
