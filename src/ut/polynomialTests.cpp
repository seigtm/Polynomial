#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <polynomial.hpp>

TEST(PolynomialTests, Equality) {
    const setm::Polynomial<uint> a({ 3 });
    const setm::Polynomial<uint> b({ 3 });

    EXPECT_EQ(a, b) << "poly == poly";
    EXPECT_EQ(a, 3) << "poly == T";
    EXPECT_EQ(3, b) << "T == poly";

    EXPECT_NE(a, b + 1) << "poly != poly";
    EXPECT_NE(a, 5) << "poly != T";
    EXPECT_NE(0, b) << "T != poly";
}

TEST(PolynomialTests, Addition) {
    {
        const setm::Polynomial<float> polynomial{ 2.0, -15.1, 8.17 };
        const setm::Polynomial<float> expected{ 3.0, -15.1, 8.17 };
        EXPECT_EQ(4 + polynomial, expected + 3) << "T + poly && poly + T";
    }
    {
        const setm::Polynomial<double> first{ 1, 2, 3 };
        const setm::Polynomial<double> second{ 1, 2, 3 };
        const setm::Polynomial<double> expected{ 2, 4, 6 };
        EXPECT_EQ(first + second, expected) << "poly + poly";
    }
}

TEST(PolynomialTests, Substraction) {
    {
        const setm::Polynomial<int> first{ 5 };
        const setm::Polynomial<int> second{ 3 };
        EXPECT_EQ(first - 2, 6 - second) << "T - poly && poly - T";
    }
    {
        const setm::Polynomial<float> minuend{ 5.1, 15.2, 20.3 };
        const setm::Polynomial<float> subtrahend{ 0.1, 0.2, 0.3 };
        const setm::Polynomial<float> expected{ 5, 15, 20 };
        EXPECT_EQ(minuend - subtrahend, expected) << "poly - poly";
    }
}

TEST(PolynomialTests, Multiplication) {
    {
        const setm::Polynomial<double> multiplier({ 1, 2, 3, 4, 5 });
        const setm::Polynomial<double> expected({ 5, 10, 15, 20, 25 });
        EXPECT_EQ((5 * multiplier), expected) << "T * poly";
        EXPECT_EQ(multiplier, (expected * 0.2)) << "poly * T";
    }
    {
        const setm::Polynomial<int> first{ 5 };
        const setm::Polynomial<int> second{ 3 };
        const setm::Polynomial<int> expected{ 15 };
        EXPECT_EQ(first * second, expected) << "poly * poly";
    }
}

TEST(PolynomialTests, Division) {
    const setm::Polynomial<double> dividend{ 10.04 };
    const setm::Polynomial<double> divisor{ 2 };
    const setm::Polynomial<double> expected{ 5.02 };
    EXPECT_EQ((10.04 / divisor), expected) << "T / poly";
    EXPECT_EQ((dividend / 2), expected) << "poly / T";
    EXPECT_EQ((dividend / divisor), expected) << "poly / poly";
}

TEST(PolynomialTests, Subscription) {
    const std::vector<int> coefficients{ 0, 1, 2, 3 };

    // Non-const polynomial [].
    setm::Polynomial<int> poly{ coefficients.cbegin(), coefficients.cend() };
    EXPECT_THAT(coefficients, ::testing::ElementsAreArray(poly.cbegin(), poly.cend()));

    poly[0] = 5;
    EXPECT_EQ(poly[0], 5) << "poly[0] = 5";

    // Const polynomial [].
    const setm::Polynomial<int> constPoly{ coefficients.cbegin(), coefficients.cend() };
    EXPECT_EQ(constPoly[0], 0) << "poly[0] == 0";
    EXPECT_THAT(coefficients, ::testing::ElementsAreArray(constPoly.cbegin(), constPoly.cend()));
}

TEST(PolynomialTests, Unary) {
    setm::Polynomial<double> poly{ -5.5, 4.4, -3.3, 2.2, -1.1, 0 };
    setm::Polynomial<double> negativePoly{ 5.5, -4.4, 3.3, -2.2, 1.1, 0 };

    EXPECT_EQ(+poly, poly) << "+poly";
    EXPECT_EQ(-poly, negativePoly) << "-poly";
}
