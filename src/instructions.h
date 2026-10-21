#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "chip8.h"
#include "stdio.h"

typedef void (*op_func)(Chip8*, uint16_t);

void code0(Chip8 *chip8, uint16_t opcode);
void code1(Chip8 *chip8, uint16_t opcode);
void code2(Chip8 *chip8, uint16_t opcode);
void code3(Chip8 *chip8, uint16_t opcode);
void code4(Chip8 *chip8, uint16_t opcode);
void code5(Chip8 *chip8, uint16_t opcode);
void code6(Chip8 *chip8, uint16_t opcode);
void code7(Chip8 *chip8, uint16_t opcode);
void code8(Chip8 *chip8, uint16_t opcode);
void code9(Chip8 *chip8, uint16_t opcode);
void codeA(Chip8 *chip8, uint16_t opcode);
void codeB(Chip8 *chip8, uint16_t opcode);
void codeC(Chip8 *chip8, uint16_t opcode);
void codeD(Chip8 *chip8, uint16_t opcode);
void codeE(Chip8 *chip8, uint16_t opcode);
void codeF(Chip8 *chip8, uint16_t opcode);

extern const op_func jump_table[16];

#endif
