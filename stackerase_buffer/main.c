#include "target_constants.h"

int sensitive_function(int *, unsigned sz);
int testcase(void) __attribute__((noinline));

int testcase(void) {
  int localbuffer[ARR_SZ];
  int idx = sensitive_function(localbuffer, sizeof(localbuffer));
  return localbuffer[idx];
}

int main(void) {
  return testcase();
}
