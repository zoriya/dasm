#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "../dasm.h"
#include "../interpretor.h"

void mov(const instruction_t *self, state_t *state)
{
	void *from = get_operand(self, 0, state);
	void *to = get_operand(self, 1, state);

	if (is_operand_wide(self, 0))
		*(uint16_t *)from = *(uint16_t *)to;
	else
		*(uint8_t *)from = *(uint8_t *)to;
}
