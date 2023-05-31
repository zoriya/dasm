#include <sys/types.h>
#include <stdio.h>
#include "dasm.h"

void print_instruction(unsigned addr)
{
	printf("%X\n", addr);
}

int dasm(char *binary)
{
	int pc = 0;

	while (*binary) {
		instruction_t inst = *binary;
		print_instruction(pc);
		pc += inst.size;
	}
	return 0;
}
