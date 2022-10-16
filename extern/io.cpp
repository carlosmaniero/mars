#include <stdio.h>

extern "C" int printint(int x) {
  fprintf(stderr, "%d\n", x);
  return 0;
}
