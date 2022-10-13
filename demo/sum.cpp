#include <iostream>

extern "C" {
    double sumTwoNumbers(double, double);
}

int main() {
    std::cout << "the sum of 3 and 4 " << sumTwoNumbers(3, 4) << std::endl;
}
