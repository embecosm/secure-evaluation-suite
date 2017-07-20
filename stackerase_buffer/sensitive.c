#include "target_constants.h"

int sensitive_function(unsigned char *arr, unsigned sz);

// The contents of this function can be replaced, but the important thing
// is that the volatile "my_secret" should be available at arr+(2*ARR_OFFS)
int sensitive_function(unsigned char *arr, unsigned sz) {
  volatile unsigned int my_secret = 0xdeadbeef;

  for (unsigned i = 0; i < sz; i++) {
    arr[i] = 0x80U + (unsigned char)(i);
  }

  return ARR_OFFS;
}
