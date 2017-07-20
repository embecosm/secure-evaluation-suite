void sensitive_function(unsigned char *arr, unsigned sz, unsigned salt);

// For this example, using a sensitive value that initially lives on the stack,
// create a temporary based on it, and use this to calculate some values
// returned to the user. Here both the stack and some registers should be
// cleared.
void sensitive_function(unsigned char *arr, unsigned sz, unsigned salt) {
  volatile unsigned int my_secret = 0xdeadbeef;
  unsigned int local_secret = my_secret ^ salt;

  for (unsigned i = 0; i < sz; i++) {
    arr[i] = (unsigned char)(local_secret >> i) + (unsigned char)(i);
  }
}
