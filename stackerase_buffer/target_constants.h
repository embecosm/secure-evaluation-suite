#if defined(__riscv)
  #define ARR_SZ 10
  #define ARR_OFFS -3
#elif defined(__x86_64)
  #define ARR_SZ 10
  #define ARR_OFFS -3
#else
  #error This test is not ported for this architecture
#endif
