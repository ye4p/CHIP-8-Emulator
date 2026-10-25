#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "chip8.h"
#include "stdio.h"

typedef void (*op_func)(Chip8*);

void code0(Chip8 *chip8);
void code1(Chip8 *chip8);
void code2(Chip8 *chip8);
void code3(Chip8 *chip8);
void code4(Chip8 *chip8);
void code5(Chip8 *chip8);
void code6(Chip8 *chip8);
void code7(Chip8 *chip8);
void code8(Chip8 *chip8);
void code9(Chip8 *chip8);
void codeA(Chip8 *chip8);
void codeB(Chip8 *chip8);
void codeC(Chip8 *chip8);
void codeD(Chip8 *chip8);
void codeE(Chip8 *chip8);
void codeF(Chip8 *chip8);

extern const op_func jump_table[16];

#endif
