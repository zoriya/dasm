#pragma once

#include "dasm.h"
#include "interpretor.h"

void mov(const instruction_t *self, state_t *state);
void push(const instruction_t *self, state_t *state);
void call(const instruction_t *self, state_t *state);
void jmp(const instruction_t *self, state_t *state);
void int_inst(const instruction_t *self, state_t *state);
void sub(const instruction_t *self, state_t *state);
