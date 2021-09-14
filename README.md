# Polynomial C++ template class

> In mathematics, a polynomial is an expression consisting of variables (also called indeterminates) and coefficients, that involves only the operations of addition, subtraction, multiplication, and non-negative integer exponentiation of variables.  
> (https://en.wikipedia.org/wiki/Polynomial)

---

## Constructors:

```c++
explicit Polynomial(T value = T(0));

Polynomial(std::initializer_list<T> init);

template<class InputIt>
Polynomial(InputIt first, InputIt last);
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

```c++
Polynomial<double> minuend{ 8.2, -3.1, 15.1, 0, 2.2 };

// subtrahend = x^3 + 15x^2 + 6.1x + 4.2.
Polynomial<double> subtrahend{ 4.2, 6.1, 15.0, 1 };

// Output: 2.2x^4 - x^3 + 0.1x^2 - 9.2x + 4.
std::cout << minuend - subtrahend;
```

```c++
// multiplier = 5x^4 + 4x^3 + 3x^2 + 2x + 1.
Polynomial<int> multiplier{ 1, 2, 3, 4, 5 };

// Output: 25x^4 + 20x^3 + 15x^2 + 10x + 5.
std::cout << 5 * multiplier;
```

```c++
// first = 8.17x^2 - 15.1x + 3.
std::vector<float> firstData = { 3.0, -15.1, 8.17 };
Polynomial<float> first(firstData.cbegin(), firstData.cend());

// second = 8.17x^2 - 15.1x + 2.
std::vector<float> secondData = { 2.0, -15.1, 8.17 };
Polynomial<float> second{ secondData.cbegin(), secondData.cend() };

// Output: true, as expected.
std::cout << std::boolalpha << (first == (second + 1));
```

---

## Cloning the repository:

```bash
bash> git clone https://github.com/seigtm/Polynomial
```

---

## Build requirements:

- [cmake](https://cmake.org/) to configure the project (minimum required version is **3.5**).
- [conan](https://conan.io/) to download all application dependencies.  
  You can install **conan** by giving a command to **pip**:
  ```bash
  bash> pip install --user conan
  ```
  To use **pip** you need to install the **python** interpreter. I highly recommend to install **python3**-based versions in order to avoid unexpected results when using **conan**.

---

## Configuring and build with conan and cmake:

To install with `conan`, execute the commands:

```bash
bash> cd Polynomial
bash> conan instal . -if build --build missing
```

To build and run the project with `cmake`, execute:

```bash
bash> cd build
bash> cmake ..
```

To install and run the application, execute:

```bash
bash> make
bash> cd bin
bash> ./main
```
