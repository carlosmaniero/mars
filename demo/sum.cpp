#include <iostream>

extern "C" {
    int entry();
}

extern "C" int printint(int x) {
  fprintf(stderr, "%d\n", x);
  return 0;
}

int main() {
    entry();
}
