#include <iostream>
#include "Calculator.h"

int main() {
    Calculator calculator;
    std::cout << "Hello, GitHub!" << '\n';
    std::cout << "10 + 5 = " << calculator.add(10, 5) << '\n';
    std::cout << "10 - 5 = " << calculator.subtract(10, 5) << '\n';
    return 0;
}
