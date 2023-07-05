#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include "dasm.h"
#include "interpretor.h"

void print_state(state_t *state)
{
	printf(
		"%04x %04x %04x %04x %04x %04x %04x %04x %c%c%c%c ",
		state->ax,
		state->bx,
		state->cx,
		state->dx,
		state->sp,
		state->bp,
		state->si,
		state->di,
		state->of ? 'O' : '-',
		state->sf ? 'S' : '-',
		state->zf ? 'Z' : '-',
		state->cf ? 'C' : '-'
	);
}

int interpret(u_int8_t *binary, unsigned long size)
{
	state_t *state = calloc(sizeof(*state), 1);
	int header_size = 0;

	if (binary[0] == 0xEB && binary[1] == 0x0E) {
		header_size = 16;
		size = binary[2] | (binary[3] << 8);
	} else if (binary[0] == 0x01 && binary[1] == 0x03) {
		header_size = binary[4];
		size = binary[8]  | (binary[9] << 8)  | (binary[10] << 16) | (binary[11] << 24);
	}

	binary += header_size;

	printf(" AX   BX   CX   DX   SP   BP   SI   DI  FLAGS IP\n");
	while (state->pc < size) {
		instruction_t inst = parse_inst(binary, size - state->pc);
		unsigned inst_size = get_inst_size(inst, binary, size - state->pc);
		if (state->pc + inst_size > size) {
			return 0;
		}
		print_state(state);
		print_instruction(state->pc, inst, inst_size, binary, false);
		state->pc += inst_size;
		binary += inst_size;
	}
	return 0;
}
