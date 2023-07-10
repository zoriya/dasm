#pragma once

#include "dasm.h"
#include "interpretor.h"

void mov(const instruction_t *self, state_t *state);
void push(const instruction_t *self, state_t *state);
void pop(const instruction_t *self, state_t *state);

void call(const instruction_t *self, state_t *state);
void ret(const instruction_t *self, state_t *state);
void jmp(const instruction_t *self, state_t *state);
void jo(const instruction_t *self, state_t *state);
void jno(const instruction_t *self, state_t *state);
void jb(const instruction_t *self, state_t *state);
void jnb(const instruction_t *self, state_t *state);
void je(const instruction_t *self, state_t *state);
void jne(const instruction_t *self, state_t *state);
void jbe(const instruction_t *self, state_t *state);
void jnbe(const instruction_t *self, state_t *state);
void js(const instruction_t *self, state_t *state);
void jns(const instruction_t *self, state_t *state);
void jpe(const instruction_t *self, state_t *state);
void jpo(const instruction_t *self, state_t *state);
void jl(const instruction_t *self, state_t *state);
void jnl_inst(const instruction_t *self, state_t *state);
void jng(const instruction_t *self, state_t *state);
void jg(const instruction_t *self, state_t *state);

void int_inst(const instruction_t *self, state_t *state);
void lea(const instruction_t *self, state_t *state);

void add(const instruction_t *self, state_t *state);
void sub(const instruction_t *self, state_t *state);
void or_inst(const instruction_t *self, state_t *state);
void xor_inst(const instruction_t *self, state_t *state);
void cmp(const instruction_t *self, state_t *state);
void test(const instruction_t *self, state_t *state);
void dec(const instruction_t *self, state_t *state);
void cbw(const instruction_t *self, state_t *state);
