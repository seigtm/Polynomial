#pragma once

#include <gtest/gtest.h>
#include <polynomial.hpp>

namespace tests {

class PolynomialTests : public ::testing::Test {
protected:
    virtual ~PolynomialTests() = default;

    void SetUp() override;

    struct {
        setm::Polynomial<int> multiplication;
        setm::Polynomial<float> additionWithEquality;
    } expected;
};

}  // namespace tests
