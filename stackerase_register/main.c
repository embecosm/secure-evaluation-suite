void sensitive_function(int *, unsigned sz, unsigned salt);
int testcase(void) __attribute__((noinline));

int testcase(void) {
  int result;
  int localbuffer[10];
  sensitive_function(localbuffer, sizeof(localbuffer), 0xcabba6e);

// Note: The location of the value of interest is compiler dependent, and
//       may need to be updated if the compiler version is updated.
//       (The value of interest to find is 0xd2060481)
#if defined(__riscv)
  asm ("sw a2, %0" : "=m" (result));
#elif defined(__x86_64)
  asm ("mov %%edx, %0" : "=m" (result));
#else
  #error This test is not ported for this architecture
#endif
  result ^= 0xcabba6e;

  return result;
}

int main(void) {
  return testcase();
}
