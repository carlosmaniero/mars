#include <iostream>

extern "C" {
    int sumTwoNumbers(int, int);
}

int main() {
    std::cout << "the sum of 3 and 4 is: " << sumTwoNumbers(3, 4) << std::endl;
}
