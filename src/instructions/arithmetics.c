#include "../dasm.h"
#include "../interpretor.h"
#include <stdint.h>

void sub(const instruction_t *self, state_t *state)
{
	void *from = get_operand(self, 0, state);
	void *to = get_operand(self, 1, state);
	unsigned old = is_operand_wide(self, 0) ? *(uint16_t *)from : *(uint8_t *)from;
	unsigned minus = is_operand_wide(self, 1) ? *(uint16_t *)to : *(uint8_t *)to;
	unsigned value = old - minus;

	if (is_operand_wide(self, 0))
		*(uint16_t *)from = value;
	else
		*(uint8_t *)from = value;

	state->of = value != (is_operand_wide(self, 0) ? *(uint16_t *)from : *(uint8_t *)from);
	state->cf = old < minus;
	state->sf = value & (is_operand_wide(self, 0) ? 0x8000 : 0x80);
	state->zf = value == 0;
	// TODO: Set AF and PF
}
