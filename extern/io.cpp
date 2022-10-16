#include <stdio.h>

extern "C" int printint(int x) {
  fprintf(stdout, "%d\n", x);
  return 0;
}
