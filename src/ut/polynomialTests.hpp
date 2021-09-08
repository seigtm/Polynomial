#pragma once

#include <gtest/gtest.h>

#include <polynomial.hpp>

using ::testing::InitGoogleTest;
using ::testing::Test;
using namespace setm;

namespace tests {

class PolynomialTests : public Test {
protected:
    virtual ~PolynomialTests() = default;

    void SetUp() override;

    struct {
        Polynomial<int> multiplication;
        Polynomial<float> additionWithEquality;
    } expected;
};

}  // namespace tests
