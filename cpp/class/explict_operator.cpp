#include <iostream>

class Fraction {
public:
    Fraction(int numerator, int denominator) : numerator(numerator), denominator(denominator) {}

    // 显式类类型转换运算符：将 Fraction 转换为 double
    explicit operator double() const {
        return static_cast<double>(numerator) / denominator;
    }

private:
    int numerator;
    int denominator;
};

int main() {
    Fraction f(3, 4);
    // 显式类型转换
    double result = static_cast<double>(f); 
    std::cout << "Fraction as double: " << result << std::endl;

    return 0;
}