#pragma once

#include <gtest/gtest.h>
#include <polynomial.hpp>

namespace tests {

class PolynomialTests : public ::testing::Test {
protected:
    virtual ~PolynomialTests() = default;

    void SetUp() override;
};

}  // namespace tests
