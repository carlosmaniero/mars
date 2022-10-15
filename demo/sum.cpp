#include <iostream>

extern "C" {
    int entry();
}

int main() {
    std::cout << "the entry function returned: " << entry() << std::endl;
}
