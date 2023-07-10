#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../dasm.h"
#include "../interpretor.h"

void mov(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	operand_t to = get_operand(self, 1, state);

	// from = to;
	write_op(from, read_op(to));
}

void push(const instruction_t *self, state_t *state)
{
	unsigned what = read_op(get_operand(self, 0, state));

	if (is_operand_wide(self, 0))
		state->memory[state->sp--] = what >> 8;
	state->memory[state->sp--] = what & 0xFF;
}

void call(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	state->memory[state->sp--] = state->pc >> 8;
	state->memory[state->sp--] = state->pc & 0xFF;
	state->pc = pc - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jmp(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	state->pc = pc - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jo(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (state->of)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jno(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (!state->of)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jb(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (state->cf)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jnb(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (!state->cf)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void je(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (state->zf)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jne(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (!state->zf)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jbe(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (state->cf || state->zf)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jnbe(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (!state->cf && !state->zf)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void js(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (state->sf)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jns(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (!state->sf)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jpe(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (state->pf)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jpo(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (!state->pf)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jl(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (state->sf != state->of)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jnl_inst(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (state->sf == state->of)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jng(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (state->zf || state->sf != state->of)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void jg(const instruction_t *self, state_t *state)
{
	int8_t diff = read_op(get_operand(self, 0, state));

	if (!state->zf && state->sf == state->of)
		state->pc += diff - get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);
}

void int_inst(const instruction_t *self, state_t *state)
{
	// I have no clue what the use of type is.
	// uint8_t type = *(uint8_t *)get_operand(self, 0, state);
	syscall_t *syscall = (syscall_t *)(state->memory + state->bx);
	uint8_t *args = state->memory + state->bx + sizeof(*syscall);

	(void)self;

	switch (syscall->type) {
	case 0x1:
		printf("<exit(%d)>\n", *args);
		exit(*args);
	case 0x2:
		printf("<fork() => %d>\n", 0);
		printf("Not implemented\n");
		break;
	case 0x4: {
		uint16_t fd = *(uint16_t *)args;
		uint16_t len = *((uint16_t *)args + 1);
		// the 2 args (a number is skiped.)
		uint16_t addr = *((uint16_t *)args + 3);
		printf("<write(%d, 0x%04x, %d)", fd, addr, len);
		fflush(stdout);
		int ret = write(fd, &state->memory[addr], len);
		printf(" => %d>\n", ret);
		state->ax = 0;
		break;
	}
	default:
		printf("Unimplemented syscall %02X\n", syscall->type);
		break;
	}
}

void lea(const instruction_t *self, state_t *state)
{
	operand_t to = get_operand(self, 0, state);
	operand_t memory = get_operand(self, 1, state);

	// to = &memory;
	write_op(to, memory.ptr - state->memory);
}
