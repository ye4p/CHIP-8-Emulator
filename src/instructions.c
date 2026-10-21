#include "instructions.h"

const op_func jump_table[16] = {
    code0, code1, code2, code3, code4, code5, code6, code7, code8, code9, codeA, codeB, codeC, codeD, codeE, codeF,
};

void code0(Chip8 *chip8, uint16_t opcode) {
    opcode = opcode & 0xFFF;
    if (opcode == 0x00E0) {
        // Clear display;
        for (size_t i = 0; i < (64 * 32); ++i) {
            chip8->framebuffer[i] = 0;
        }
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
    if (chip8->V[(opcode & 0x0F00) >> 8] == opcode & 0xFF) {
       chip8->pc += 2; 
    }
}

void code4(Chip8 *chip8, uint16_t opcode) {
    if (chip8->V[(opcode & 0x0F00) >> 8] != opcode & 0xFF) {
        chip8->pc += 2;
    }
}

void code5(Chip8 *chip8, uint16_t opcode) {
    if (chip8->V[(opcode & 0x0F00) >> 8] == chip8->V[(opcode & 0x00F0) >> 4]) {
        chip8->pc += 2;
    }
}

void code6(Chip8 *chip8, uint16_t opcode) {
    chip8->V[(opcode & 0x0F00) >> 8] == opcode & 0xFF; 
}

void code7(Chip8 *chip8, uint16_t opcode) {
    chip8->V[(opcode & 0x0F00) >> 8] += opcode & 0xFF;
}

void code8(Chip8 *chip8, uint16_t opcode) {
    switch (opcode & 0xF) {
        case 0:
            chip8->V[(opcode & 0x0F00) >> 8] = chip8->V[(opcode & 0x00F0) >> 4];
            break;
        case 1:
            chip8->V[(opcode & 0x0F00) >> 8] |= chip8->V[(opcode & 0x00F0) >> 4];
            break;
        case 2:
            chip8->V[(opcode & 0x0F00) >> 8] &= chip8->V[(opcode & 0x00F0) >> 4];
            break;
        case 3:
            chip8->V[(opcode & 0x0F00) >> 8] ^= chip8->V[(opcode & 0x00F0) >> 4];
            break;
        case 4:
            uint16_t res = chip8->V[(opcode & 0x0F00) >> 8] + chip8->V[(opcode & 0x00F0) >> 4];
            if (res > 255) {
                chip8->V[0xF] = 1;
            }
            chip8->V[(opcode & 0x0F00) >> 8] = res;
            break;
        case 5:
            if (chip8->V[(opcode & 0x0F00) >> 8] > chip8->V[(opcode & 0x00F0) >> 4]) {
                chip8->V[0xF] = 1;
            }
            chip8->V[(opcode & 0x0F00) >> 8] -= chip8->V[(opcode & 0x00F0) >> 4];
            break;
        case 6:
            if (chip8->V[(opcode & 0x0F00) >> 8] & 0x1) {
                chip8->V[0xF] = 1;
            } else {
                chip8->V[0xF] = 0;
            }
            break; 
        case 7:
            if (chip8->V[(opcode & 0x00F0) >> 4] > chip8->V[(opcode & 0x0F00) >> 8]) {
                chip8->V[0xF] = 1;
            } else {
                chip8->V[0xF] = 0;
            }
            chip8->V[(opcode & 0x0F00) >> 8] = chip8->V[(opcode & 0x00F0) >> 4] - chip8->V[opcode << 4 >> 12];
            break;
        case 0xE:
            if (chip8->V[(opcode & 0x0F00) >> 8] & 0b10000000) {
                chip8->V[0xF] = 1;
            } else {
                chip8->V[(opcode & 0x0F00) >> 8] *= 2;
            }
            break;
    }
}

void code9(Chip8 *chip8, uint16_t opcode) {
    if (chip8->V[(opcode & 0x0F00) >> 8] != chip8->V[(opcode & 0x00F0) >> 4]) {
        chip8->pc += 2;
    }
}

void codeA(Chip8 *chip8, uint16_t opcode) {
    chip8->I = opcode & 0xFFF;
}

void codeB(Chip8 *chip8, uint16_t opcode) {
    chip8->pc = (opcode & 0xFFF) + chip8->V[0];
}

void codeC(Chip8 *chip8, uint16_t opcode) {
    uint8_t random_number = rand() % 256;
    chip8->V[(opcode & 0x0F00) >> 8] = random_number & (opcode & 0xFF);
}

void codeD(Chip8 *chip8, uint16_t opcode) {
    // TODO
}

void codeE(Chip8 *chip8, uint16_t opcode) {
    if (opcode & 0xFF == 0x9E) {
        if (kbhit == chip8->V[(opcode & 0x0F00) >> 8]) {
            chip8->pc += 2;
        }
    } else {
        if (kbhit != chip8->V[(opcode & 0x0F00) >> 8]) {
            chip8->pc += 2;
        }
    }
}

void codeF(Chip8 *chip8, uint16_t opcode) {
    switch (opcode & 0xFF) {
        case 0x07:
            chip8->V[(opcode & 0x0F00) >> 8] = chip8->delay_timer;
            break;
        case 0x0A:
            int temp = getchar();
            chip8->V[(opcode & 0x0F00) >> 8] = temp;
            break;
        case 0x15:
            chip8->delay_timer = chip8->V[(opcode & 0x0F00) >> 8];
            break;
        case 0x18:
            chip8->sound_timer = chip8->V[(opcode & 0x0F00) >> 8];
            break;
        case 0x1E:
            chip8->I += chip8->V[(opcode & 0x0F00) >> 8];
            break;
        case 0x29:
            // TODO
            break;
        case 0x33:
            uint8_t v = chip8->V[(opcode & 0x0F00) >> 8];
            for (size_t i = 2; i >= 0; --i) {
                chip8->memory[chip8->I + i] = v % 10;
                v /= 10;
            }
            break;
        case 0x55:
            for (size_t i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                chip8->memory[chip8->I + i] = chip8->V[i];
            }
            break;
        case 0x65:
            for (size_t i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                chip8->V[i] = chip8->memory[chip8->I + i];
            }
            break;
    }
}


