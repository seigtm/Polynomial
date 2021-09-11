#pragma once

#include <cmath>

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <string>
#include <sstream>
#include <type_traits>
#include <vector>

namespace setm {

template<class T>
class Polynomial {
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");

public:
    using value_type = T;
    using reference = typename std::vector<T>::reference;
    using const_reference = typename std::vector<T>::const_reference;
    using pointer = typename std::vector<T>::pointer;
    using const_pointer = typename std::vector<T>::const_pointer;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    Polynomial(T data = T(0))
        : coefficients{ data } {}

    Polynomial(std::initializer_list<T> data)
        : coefficients{ data } {
        normalize(coefficients);
    }

    template<class InputIt>
    Polynomial(InputIt first, InputIt last)
        : coefficients{ first, last } {
        normalize(coefficients);
    }

    // Addition operators.
    Polynomial &operator+=(const Polynomial &other) {
        std::transform(coefficients.cbegin(), coefficients.cend(), other.coefficients.cbegin(), coefficients.begin(), std::plus<>{});
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
        std::transform(coefficients.cbegin(), coefficients.cend(), other.coefficients.cbegin(), coefficients.begin(), std::minus<>{});
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
        return (result *= Polynomial({ value }));
    }

    friend Polynomial operator*(T value, const Polynomial &polynomial) {
        return (polynomial * Polynomial({ value }));
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
        return (result /= Polynomial({ value }));
    }

    friend Polynomial operator/(T value, const Polynomial &polynomial) {
        return (Polynomial({ value }) / polynomial);
    }

    // Comparison operators.
    bool operator==(const Polynomial &other) const noexcept {
        return std::equal(coefficients.cbegin(), coefficients.cend(), other.coefficients.begin());
    }

    bool operator!=(const Polynomial &other) const noexcept {
        return !(*this == other);
    }

    bool operator==(const T other) const {
        return *this == Polynomial({ other });
    }

    bool operator!=(const T other) const {
        return *this != Polynomial({ other });
    }

    friend bool operator==(const T other, const Polynomial &polynomial) {
        return polynomial == other;
    }

    friend bool operator!=(const T other, const Polynomial &polynomial) {
        return polynomial != other;
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
        std::stringstream strout{};

        for(auto iter = polynomial.coefficients.rbegin(); iter != polynomial.coefficients.rend(); ++iter, --power) {
            T koef = *iter;

            if(koef == T())
                continue;

            if(koef > T()) {
                if(!firstIteration) {
                    strout << "+ ";
                }
            } else {
                strout << "- ";
            }

            firstIteration = false;

            if((koef != T(1) && koef != T(-1)) || power == 0)
                strout << fabs(koef);

            if(power != 0) {
                strout << ((power == 1) ? "x" : "x^" + std::to_string(power));
            }

            strout << " ";
        }

        if(polynomial.coefficients.size() == 0)
            strout << 0;

        out << strout.str();
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
