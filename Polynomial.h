#pragma once
#include <cmath>
#include <vector>
#include <sstream>
#include <algorithm>

template <class T>
class Polynomial
{
public:
  Polynomial(const std::vector<T> &data)
      : coefficients(data)
  {
    normalize(coefficients);
  }

  Polynomial(const T &value = T())
      : Polynomial(std::vector<T>({value}))
  {
  }

  Polynomial(std::initializer_list<T> &initlist)
      : coefficients(initlist)
  {
  }

  // Addition operators.
  Polynomial &operator+=(const Polynomial &other)
  {

    coefficients.resize(std::max(other.coefficients.size(), coefficients.size()), T());

    for (size_t i = 0; i < std::min(coefficients.size(), other.coefficients.size()); ++i)
    {
      coefficients[i] += other.coefficients[i];
    }

    normalize(coefficients);
    return *this;
  }
  Polynomial operator+(const Polynomial &other)
  {
    Polynomial result(coefficients);
    return (result += other);
  }
  Polynomial operator+(T value)
  {
    Polynomial result(coefficients);
    return (result += Polynomial(value));
  }
  friend Polynomial operator+(T value, Polynomial polynomial)
  {
    return (polynomial + value);
  }

  // Substraction operators.
  Polynomial &operator-=(const Polynomial &other)
  {

    coefficients.resize(std::max(other.coefficients.size(), coefficients.size()), T());

    for (size_t i = 0; i < std::min(coefficients.size(), other.coefficients.size()); ++i)
    {
      coefficients[i] -= other.coefficients[i];
    }

    normalize(coefficients);
    return *this;
  }
  Polynomial operator-(Polynomial &other)
  {
    Polynomial result(coefficients);
    return (result -= other);
  }
  Polynomial operator-(T value)
  {
    Polynomial result(coefficients);
    return (result -= Polynomial(value));
  }
  friend Polynomial operator-(T value, Polynomial polynomial)
  {
    return (polynomial - value);
  }

  // Multiplication operators.
  Polynomial &operator*=(const Polynomial &other)
  {
    std::vector<T> temporary(coefficients.size() + other.coefficients.size(),
                             T());

    for (size_t i{}; i < coefficients.size(); ++i)
    {
      for (size_t j{}; j < other.coefficients.size(); ++j)
      {
        temporary[i + j] += coefficients[i] * other.coefficients[j];
      }
    }

    normalize(temporary);
    *this = Polynomial(temporary);
    return *this;
  }

  Polynomial operator*(const Polynomial &other)
  {
    Polynomial result(coefficients);
    return (result *= other);
  }

  Polynomial operator*(T value)
  {
    Polynomial result(coefficients);
    return (result *= Polynomial(value));
  }

  friend Polynomial operator*(T value, Polynomial polynomial)
  {
    return (polynomial * value);
  }

  // Division operators.
  Polynomial &operator/=(const Polynomial &other)
  {
    Polynomial<T> priv(T(0));
    while (coefficients.size() >= other.coefficients.size())
    {
      T coef = coefficients.back() / other.coefficients.back();
      size_t degree = coefficients.size() - other.coefficients.size();
      std::vector<T> div(degree + 1);
      div.back() = coef;
      Polynomial<T> temporary(div);
      *this -= temporary * other;
      priv += temporary;
    }
    coefficients = priv.coefficients;
    return *this;
  }

  Polynomial operator/(const Polynomial &other)
  {
    Polynomial result(coefficients);
    return (result /= other);
  }

  Polynomial operator/(T value)
  {
    Polynomial result(coefficients);
    return (result /= Polynomial(value));
  }

  friend Polynomial operator/(T value, Polynomial polynomial)
  {
    return (Polynomial(value) / polynomial);
  }

  // Comparison operators.
  bool operator==(const Polynomial &other) const
  {
    return (coefficients == other.coefficients);
  }

  bool operator!=(const Polynomial &other) const
  {
    return (coefficients != other.coefficients);
  }
  bool operator==(const T &other) const
  {
    return *this == Polynomial(other);
  }
  bool operator!=(const T &other) const
  {
    return *this != Polynomial(other);
  }
  friend bool operator==(const T &other, const Polynomial &polynomial)
  {
    return polynomial == other;
  }
  friend bool operator!=(const T &other, const Polynomial &polynomial)
  {
    return polynomial != other;
  }

  // Unary operators.
  Polynomial operator-() const
  {
    Polynomial result{*this};
    for (auto &el : result.coefficients)
      el *= -1;
    return result;
  }

  Polynomial operator+() const
  {
    return Polynomial(*this);
  }

  T &operator[](const size_t index)
  {
    return coefficients[index];
  }

  const T &operator[](const size_t index) const
  {
    return coefficients[index];
  }

  // Bitwise operators.
  friend std::ostream &operator<<(std::ostream &out, const Polynomial &polynomial)
  {
    size_t power{polynomial.coefficients.size() - 1};
    bool firstIteration{true};
    std::stringstream strout{};

    for (auto iter = polynomial.coefficients.rbegin(); iter != polynomial.coefficients.rend(); ++iter, --power)
    {

      T koef = *iter;

      if (koef == T())
        continue;

      if (koef > T())
      {
        if (!firstIteration)
        {
          strout << "+ ";
        }
      }
      else
      {
        strout << "- ";
      }

      firstIteration = false;

      if ((koef != T(1) && koef != T(-1)) || power == 0)
        strout << fabs(koef);

      if (power != 0)
      {
        strout << ((power == 1) ? "x" : "x^" + std::to_string(power));
      }

      strout << " ";
    }

    if (polynomial.coefficients.size() == 0)
      strout << 0;

    out << strout.str();
    return out;
  }
  friend std::istream &operator>>(std::istream &in, Polynomial &polynomial)
  {
    T value;
    while (true)
    {
      in >> value;
      polynomial.coefficients.push_back(value);
      if (in.get() == '\n')
      {
        return in;
      }
    }
    return in;
  }

private:
  std::vector<T> coefficients;

  // Comparing two numbers.
  bool isEqual(const T &a, const T &b)
  {
    return a == b;
  }
  bool isEqual(double &a, double &b)
  {
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
  }
  bool isEqual(float &a, float &b)
  {
    return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
  }

  // Normalizes the polynomial by removing the side zeros.
  void normalize(std::vector<T> &coefs)
  {

    while (!coefs.empty())
    {
      if (!isEqual(coefs.back(), T()))
      {
        return;
      }
      coefs.pop_back();
    }
  }
};
