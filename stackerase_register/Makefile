all: x86

x86:
	x86_64-linux-gnu-gcc main.c sensitive.c -O2 -g -o x86.out

riscv:
	riscv32-unknown-elf-gcc main.c sensitive.c -O2 -g -o riscv.out

clean:
	rm -f x86.out riscv.out
