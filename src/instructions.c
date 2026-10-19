#include "instructions.h"

void code0(Chip8 *chip8, uint16_t opcode) {
    opcode = opcode & 0xFFF;
    if (opcode == 0x00E0) {
        // Clear display;
    } else if (opcode = 0x00EE) {
        chip8->pc = chip8->stack[chip8->sp];
    }
    // There is also 0nnn operation, but it is deprecated
}

void code1(Chip8 *chip8, uint16_t opcode) {
    chip8->pc = opcode & 0xFFF;
}

void code2(Chip8 *chip8, uint16_t opcode) {
    chip8->stack[++chip8->sp] = chip8->pc;
    chip8->pc = opcode & 0xFFF;
}

void code3(Chip8 *chip8, uint16_t opcode) {
    if (chip8->V[opcode << 4 >> 12] == opcode & 0xFF) {
       chip8->pc += 2; 
    }
}

void code4(Chip8 *chip8, uint16_t opcode) {
    if (chip8->V[opcode << 4 >> 12] != opcode & 0xFF) {
        chip->pc += 2;
    }
}

void code5(Chip8 *chip8, uint16_t opcode) {
    if (Chip8->V[opcode << 4 >> 12] == Chip8->V[opcode << 8 >> 12]) {
        chip->pc += 2;
    }
}

void code6(Chip8 *chip8, uint16_t opcode) {
    chip8->V[opcode << 4 >> 12] == opcode & 0xFF; 
}

void code7(Chip8 *chip8, uint16_t opcode) {
    chip8->V[opcode << 4 >> 12] += opcode & 0xFF;
}

void code8(Chip8 *chip8, uint16_t opcode) {
    switch (opcode & 0xF)
        case 0:
            chip8->V[opcode << 4 >> 12] = chip8->V[opcode << 8 >> 12];
            break;
        case 1:
            chip8->V[opcode << 4 >> 12] |= chip8->V[opcode << 8 >> 12];
            break;
        case 2:
            chip8->V[opcode << 4 >> 12] &= chip8->V[opcode << 8 >> 12];
            break;
        case 3:
            chip8->V[opcode << 4 >> 12] ^= chip8->V[opcode << 8 >> 12];
            break;
        case 4:
            uint16_t res = chip8->V[opcode << 4 >> 12] + chip8->V[opcode << 8 >> 12];
            if (res > 255) {
                chip8->V[0xF] = 1;
            }
            chip8->V[opcode << 4 >> 12] = res;
            break;
        case 5:
            if (chip8->[opcode << 4 >> 12] > chip8->[opcode << 8 >> 12]) {
                chip8->V[0xF] = 1;
            }
            chip8->V[opcode << 4 >> 12] -= chip8->[opcode << 8 >> 12];
            break;
        case 6:
            break; 
}
