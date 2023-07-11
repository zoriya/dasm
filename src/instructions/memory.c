#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
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
		state->memory[--state->sp] = what >> 8;
	state->memory[--state->sp] = what & 0xFF;
}

void pop(const instruction_t *self, state_t *state)
{
	operand_t where = get_operand(self, 0, state);
	unsigned what = 0;

	what |= state->memory[state->sp++];
	if (is_operand_wide(self, 0))
		what |= state->memory[state->sp++] << 8;
	write_op(where, what);
}

void call(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));
	unsigned inst_size = get_inst_size(*self, state->binary + state->pc, state->binary_size - state->pc);

	state->pc += inst_size;
	state->memory[--state->sp] = state->pc >> 8;
	state->memory[--state->sp] = state->pc & 0xFF;
	state->pc = pc;
}

void ret(const instruction_t *self, state_t *state)
{
	uint16_t spmin = self->mode[0] != END ? read_op(get_operand(self, 0, state)) : 0;
	unsigned what = 0;

	what |= state->memory[state->sp++];
	if (is_operand_wide(self, 0))
		what |= state->memory[state->sp++] << 8;
	state->pc = what;
	state->sp += spmin;
}

void jmp(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	state->pc = pc;
}

void jo(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (state->of)
		state->pc = pc;
}

void jno(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (!state->of)
		state->pc = pc;
}

void jb(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (state->cf)
		state->pc = pc;
}

void jnb(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (!state->cf)
		state->pc = pc;
}

void je(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (state->zf)
		state->pc = pc;
}

void jne(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (!state->zf)
		state->pc = pc;
}

void jbe(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (state->cf || state->zf)
		state->pc = pc;
}

void jnbe(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (!state->cf && !state->zf)
		state->pc = pc;
}

void js(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (state->sf)
		state->pc = pc;
}

void jns(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (!state->sf)
		state->pc = pc;
}

void jpe(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (state->pf)
		state->pc = pc;
}

void jpo(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (!state->pf)
		state->pc = pc;
}

void jl(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (state->sf != state->of)
		state->pc = pc;
}

void jnl_inst(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (state->sf == state->of)
		state->pc = pc;
}

void jng(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (state->zf || state->sf != state->of)
		state->pc = pc;
}

void jg(const instruction_t *self, state_t *state)
{
	uint16_t pc = read_op(get_operand(self, 0, state));

	if (!state->zf && state->sf == state->of)
		state->pc = pc;
}

void int_inst(const instruction_t *self, state_t *state)
{
	// I have no clue what the use of type is.
	// uint8_t type = *(uint8_t *)get_operand(self, 0, state);
	syscall_t *syscall = (syscall_t *)(state->memory + state->bx);
	uint8_t *args = state->memory + state->bx + sizeof(*syscall);

	(void)self;

	state->ax = 0;

	switch (syscall->type) {
	case 0x1:
		if (state->parse_data.debug)
			printf("<exit(%d)>\n", *args);
		exit(*args);
	case 0x2:
		if (state->parse_data.debug)
			printf("<fork() => %d>\n", 0);
		printf("Not implemented\n");
		break;
	case 0x4: {
		uint16_t fd = *(uint16_t *)args;
		uint16_t len = *((uint16_t *)args + 1);
		// the 2 args (a number is skiped.)
		uint16_t addr = *((uint16_t *)args + 3);
		if (state->parse_data.debug)
			printf("<write(%d, 0x%04x, %d)", fd, addr, len);
		fflush(stdout);
		int ret = write(fd, &state->memory[addr], len);
		if (state->parse_data.debug)
			printf(" => %d>\n", ret);
		syscall->type = ret;
		break;
	}
	case 0x11: {
		uint16_t addr = *((uint16_t *)args + 3);
		if (state->parse_data.debug)
			printf("<brk(0x%04x)", addr);
		uint16_t ret;
		if (addr < state->data_size || (addr >= ((state->sp & ~0x3ff) - 0x400))) {
			ret = -ENOMEM;
			if (state->parse_data.debug)
				printf(" => ENOMEM>\n");
		} else {
			ret = 0;
			if (state->parse_data.debug)
				printf(" => 0>\n");
		}
		*((uint16_t *)args + 7) = addr;
		syscall->type = ret;
		break;
	}
	case 0x36: {
		uint16_t fd = *(uint16_t *)args;
		uint16_t addr = *((uint16_t *)args + 2);
		uint16_t addr2 = *((uint16_t *)args + 7);
		if (state->parse_data.debug)
			printf("<ioctl(%d, 0x%04x, 0x%04x)>\n", fd, addr, addr2);
		int16_t ret = -EINVAL;
		syscall->type = ret;
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
