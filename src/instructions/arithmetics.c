#include "../dasm.h"
#include "../interpretor.h"
#include <stdint.h>

void sub(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	operand_t to = get_operand(self, 1, state);
	unsigned old = read_op(from);
	unsigned minus = read_op(to);
	unsigned value = old - minus;

	write_op(from, value);

	state->of = value != read_op(from);
	state->cf = old < minus;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set AF and PF
}

void xor_inst(const instruction_t *self, state_t *state)
{
	operand_t from = get_operand(self, 0, state);
	operand_t to = get_operand(self, 1, state);
	unsigned old = read_op(from);
	unsigned minus = read_op(to);
	unsigned value = old ^ minus;

	write_op(from, value);

	state->of = 0;
	state->cf = 0;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set PF

}
