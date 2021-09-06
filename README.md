# Polynomial C++ template class

> In mathematics, a polynomial is an expression consisting of variables (also called indeterminates) and coefficients, that involves only the operations of addition, subtraction, multiplication, and non-negative integer exponentiation of variables.  
> (https://en.wikipedia.org/wiki/Polynomial)

---

## Constructors:

```c++
Polynomial(const std::vector<T>& data)
    : coefficients(data)
{
    normalize(coefficients);
}

Polynomial(const T& value = T())
    : Polynomial(std::vector<T>({value}))
{}

Polynomial(std::initializer_list<T> &initlist)
    : coefficients(initlist)
{}
```

---

## List of supported operators and functions:

- unary;
- addition;
- substraction;
- multiplication;
- division;
- comparison (equality);
- bitwise (for std::cin and std::cout).

---

## Usage examples:

``` c++
// minuend = 2.2x^4 + 15.1x^2 - 3.1x + 8.2.
Polynomial<double> minuend({8.2, -3.1, 15.1, 0, 2.2}); 

// subtrahend = x^3 + 15x^2 + 6.1x + 4.2.
Polynomial<double> subtrahend({4.2, 6.1, 15.0, 1});

// Output: 2.2x^4 - x^3 + 0.1x^2 - 9.2x + 4.
std::cout << minuend - subtrahend; 
```

``` c++
// multiplier = 5x^4 + 4x^3 + 3x^2 + 2x + 1.
Polynomial<int> multiplier({1, 2, 3, 4, 5});

// Output: 25x^4 + 20x^3 + 15x^2 + 10x + 5.
std::cout << 5 * multiplier;
```

``` c++
// first = 8.17x^2 - 15.1x + 3.
std::vector<float> firstData = {3.0, -15.1, 8.17};
Polynomial<float> first(firstData);

// second = 8.17x^2 - 15.1x + 2.
std::vector<float> secondData = {2.0, -15.1, 8.17};
Polynomial<float> second(secondData);

// Output: true, as expected.
std::cout << std::boolalpha << (first == (second + 1));
```
