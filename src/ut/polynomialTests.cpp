#include "polynomialTests.hpp"

namespace tests {

void PolynomialTests::SetUp() {
    expected.multiplication = setm::Polynomial<int>({ 5, 10, 15, 20, 25 });
    expected.additionWithEquality = setm::Polynomial<float>({ 3.0, -15.1, 8.17 });
}

TEST_F(PolynomialTests, additionWithEquality) {
    setm::Polynomial<float> polynomial({ 2.0, -15.1, 8.17 });

    ASSERT_EQ(polynomial + 1, expected.additionWithEquality);
}

TEST_F(PolynomialTests, multiplication) {
    setm::Polynomial<int> multiplier({ 1, 2, 3, 4, 5 });

    ASSERT_EQ((5 * multiplier), expected.multiplication);
}

}  // namespace tests
