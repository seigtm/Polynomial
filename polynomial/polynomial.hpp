#pragma once

#include <cmath>

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <string>
#include <sstream>
#include <type_traits>
#include <vector>

#include <boost/type_traits.hpp>

namespace setm {

template<class T>
class Polynomial {
    // Polynomial class template parameter
    //  must support all of the operations listed below.
    static_assert(
        // Arithmetic.
        boost::has_plus<T>{} &&
            boost::has_minus<T>{} &&
            boost::has_multiplies<T>{} &&
            boost::has_divides<T>{} &&
            // Assign.
            boost::has_plus_assign<T>{} &&
            boost::has_minus_assign<T>{} &&
            boost::has_multiplies_assign<T>{} &&
            boost::has_divides_assign<T>{} &&
            // Unary.
            boost::has_unary_minus<T>{} &&
            boost::has_unary_plus<T>{} &&
            // Equality.
            boost::has_equal_to<T>{} &&
            boost::has_not_equal_to<T>{},
        "T must support arithmetic, assign, unary and equality operators!");

public:
    using value_type = T;
    using reference = typename std::vector<T>::reference;
    using const_reference = typename std::vector<T>::const_reference;
    using pointer = typename std::vector<T>::pointer;
    using const_pointer = typename std::vector<T>::const_pointer;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    explicit Polynomial(T value = T(0))
        : coefficients{ value } {
        normalize(coefficients);
    }

    Polynomial(std::initializer_list<T> init)
        : coefficients{ init } {
        normalize(coefficients);
    }

    template<class InputIt>
    Polynomial(InputIt first, InputIt last)
        : coefficients{ first, last } {
        normalize(coefficients);
    }

    // Addition operators.
    Polynomial &operator+=(const Polynomial &other) {
        coefficients.resize(std::max(other.coefficients.size(), coefficients.size()));
        std::transform(coefficients.cbegin(), coefficients.cbegin() + other.coefficients.size(), other.coefficients.cbegin(), coefficients.begin(), std::plus<>{});
        normalize(coefficients);
        return *this;
    }

    Polynomial operator+(const Polynomial &other) const {
        Polynomial result{ coefficients.cbegin(), coefficients.cend() };
        return (result += other);
    }

    Polynomial operator+(T value) const {
        Polynomial result{ coefficients.cbegin(), coefficients.cend() };
        return (result += Polynomial{ value });
    }

    friend Polynomial operator+(T value, Polynomial polynomial) {
        return (polynomial + value);
    }

    // Substraction operators.
    Polynomial &operator-=(const Polynomial &other) {
        coefficients.resize(std::max(other.coefficients.size(), coefficients.size()));
        std::transform(coefficients.cbegin(), coefficients.cbegin() + other.coefficients.size(), other.coefficients.cbegin(), coefficients.begin(), std::minus<>{});
        normalize(coefficients);
        return *this;
    }

    Polynomial operator-(const Polynomial &other) const {
        Polynomial result{ coefficients.cbegin(), coefficients.cend() };
        return (result -= other);
    }

    Polynomial operator-(T value) const {
        Polynomial result{ coefficients.cbegin(), coefficients.cend() };
        return (result -= Polynomial{ value });
    }

    friend Polynomial operator-(T value, Polynomial polynomial) {
        return -(polynomial - value);
    }

    // Multiplication operators.
    Polynomial &operator*=(const Polynomial &other) {
        std::vector<T> temporary(coefficients.size() + other.coefficients.size(),
                                 T());

        for(size_t i{}; i < coefficients.size(); ++i) {
            for(size_t j{}; j < other.coefficients.size(); ++j) {
                temporary[i + j] += coefficients[i] * other.coefficients[j];
            }
        }

        normalize(temporary);
        coefficients = std::move(temporary);
        return *this;
    }

    Polynomial operator*(const Polynomial &other) const {
        Polynomial result{ coefficients.cbegin(), coefficients.cend() };
        return (result *= other);
    }

    Polynomial operator*(T value) const {
        Polynomial result{ coefficients.cbegin(), coefficients.cend() };
        return (result *= Polynomial{ value });
    }

    friend Polynomial operator*(T value, const Polynomial &polynomial) {
        return (polynomial * Polynomial{ value });
    }

    // Division operators.
    Polynomial &operator/=(const Polynomial &other) {
        Polynomial<T> priv({ T(0) });

        while(coefficients.size() >= other.coefficients.size()) {
            T coef = coefficients.back() / other.coefficients.back();
            size_t degree = coefficients.size() - other.coefficients.size();
            std::vector<T> div(degree + 1);
            div.back() = coef;
            Polynomial<T> temporary(div.cbegin(), div.cend());
            *this -= temporary * other;
            priv += temporary;
        }

        coefficients = std::move(priv.coefficients);
        return *this;
    }

    Polynomial operator/(const Polynomial &other) const {
        Polynomial result{ coefficients.cbegin(), coefficients.cend() };
        return (result /= other);
    }

    Polynomial operator/(T value) const {
        Polynomial result{ coefficients.cbegin(), coefficients.cend() };
        return (result /= Polynomial{ value });
    }

    friend Polynomial operator/(T value, const Polynomial &polynomial) {
        return (Polynomial{ value } / polynomial);
    }

    // Comparison operators.
    bool operator==(const Polynomial &other) const noexcept {
        return coefficients == other.coefficients;
    }

    bool operator!=(const Polynomial &other) const noexcept {
        return !(*this == other);
    }

    bool operator==(T other) const noexcept {
        return *this == Polynomial{ other };
    }

    bool operator!=(T other) const noexcept {
        return !(*this == Polynomial{ other });
    }

    friend bool operator==(T other, const Polynomial &polynomial) noexcept {
        return polynomial == other;
    }

    friend bool operator!=(T other, const Polynomial &polynomial) noexcept {
        return !(polynomial == other);
    }

    // Unary operators.
    Polynomial operator-() const {
        Polynomial result{ *this };
        std::transform(result.coefficients.cbegin(), result.coefficients.cend(), result.coefficients.begin(), std::negate<>{});
        return result;
    }

    Polynomial operator+() const {
        return *this;
    }

    // Subscript operators.
    T &operator[](size_t index) noexcept {
        return coefficients[index];
    }

    T operator[](size_t index) const {
        return coefficients[index];
    }

    // Bitwise operators.
    friend std::ostream &operator<<(std::ostream &out, const Polynomial &polynomial) {
        size_t power{ polynomial.coefficients.size() - 1 };
        bool firstIteration{ true };

        for(auto iter = polynomial.coefficients.rbegin(); iter != polynomial.coefficients.rend(); ++iter, --power) {
            T koef = *iter;

            if(koef == T())
                continue;

            if constexpr(std::is_arithmetic_v<T>) {
                if(koef > T()) {
                    if(!firstIteration) {
                        out << "+ ";
                    }
                } else {
                    out << "- ";
                }
            } else {
                if(!firstIteration) {
                    out << "+ ";
                }
            }

            firstIteration = false;

            if constexpr(std::is_arithmetic_v<T>) {
                if((koef != T(1) && koef != T(-1)) || power == 0)
                    out << fabs(koef);
            } else {
                out << koef;
            }

            if(power != 0) {
                out << ((power == 1) ? "x" : "x^" + std::to_string(power));
            }

            out << " ";
        }

        if(polynomial.coefficients.empty())
            out << 0;

        return out;
    }

    friend std::istream &operator>>(std::istream &in, Polynomial &polynomial) {
        polynomial.coefficients.clear();
        std::string line{};

        if(std::getline(in, line)) {
            std::istringstream iss{ line };
            std::copy(std::istream_iterator<T>{ iss },
                      std::istream_iterator<T>(),
                      std::back_inserter(polynomial.coefficients));
        }

        return in;
    }

    // Random access iterators.
    auto begin() noexcept {
        return coefficients.begin();
    }

    auto begin() const noexcept {
        return coefficients.begin();
    }

    auto cbegin() const noexcept {
        return coefficients.cbegin();
    }

    auto end() noexcept {
        return coefficients.end();
    }

    auto end() const noexcept {
        return coefficients.end();
    }

    auto cend() const noexcept {
        return coefficients.cend();
    }

    auto size() const noexcept {
        return coefficients.size();
    }

private:
    std::vector<T> coefficients;

    // Normalizes the polynomial by removing the side zeros.
    void normalize(std::vector<T> &coefs) {
        coefs.erase(std::find_if(coefs.rbegin(), coefs.rend(), [](auto v) { return 0 != v; }).base(), coefs.end());
    }
};

}  // namespace setm
