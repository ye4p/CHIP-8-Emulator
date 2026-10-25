#include "instructions.h"

const op_func jump_table[16] = {
    code0, code1, code2, code3, code4, code5, code6, code7, code8, code9, codeA, codeB, codeC, codeD, codeE, codeF,
};

void code0(Chip8 *chip8) {
    chip8->opcode = chip8->opcode & 0xFFF;
    if (chip8->opcode == 0x00E0) {
        // Clear display;
        memset(chip8->framebuffer, 0, 64 * 32);

   } else if (chip8->opcode = 0x00EE) {
        chip8->pc = chip8->stack[chip8->sp];
    }
    // There is also 0nnn operation, but it is deprecated
}

void code1(Chip8 *chip8) {
    chip8->pc = chip8->opcode & 0xFFF;
}

void code2(Chip8 *chip8) {
    chip8->stack[++chip8->sp] = chip8->pc;
    chip8->pc = chip8->opcode & 0xFFF;
}

void code3(Chip8 *chip8) {
    if (chip8->V[(chip8->opcode & 0x0F00) >> 8] == chip8->opcode & 0xFF) {
       chip8->pc += 2; 
    }
}

void code4(Chip8 *chip8) {
    if (chip8->V[(chip8->opcode & 0x0F00) >> 8] != chip8->opcode & 0xFF) {
        chip8->pc += 2;
    }
}

void code5(Chip8 *chip8) {
    if (chip8->V[(chip8->opcode & 0x0F00) >> 8] == chip8->V[(chip8->opcode & 0x00F0) >> 4]) {
        chip8->pc += 2;
    }
}

void code6(Chip8 *chip8) {
    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->opcode & 0xFF; 
}

void code7(Chip8 *chip8) {
    chip8->V[(chip8->opcode & 0x0F00) >> 8] += chip8->opcode & 0xFF;
}

void code8(Chip8 *chip8) {
    switch (chip8->opcode & 0xF) {
        case 0:
            chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x00F0) >> 4];
            break;
        case 1:
            chip8->V[(chip8->opcode & 0x0F00) >> 8] |= chip8->V[(chip8->opcode & 0x00F0) >> 4];
            break;
        case 2:
            chip8->V[(chip8->opcode & 0x0F00) >> 8] &= chip8->V[(chip8->opcode & 0x00F0) >> 4];
            break;
        case 3:
            chip8->V[(chip8->opcode & 0x0F00) >> 8] ^= chip8->V[(chip8->opcode & 0x00F0) >> 4];
            break;
        case 4:
            uint16_t res = chip8->V[(chip8->opcode & 0x0F00) >> 8] + chip8->V[(chip8->opcode & 0x00F0) >> 4];
            if (res > 255) {
                chip8->V[0xF] = 1;
            }
            chip8->V[(chip8->opcode & 0x0F00) >> 8] = res;
            break;
        case 5:
            if (chip8->V[(chip8->opcode & 0x0F00) >> 8] > chip8->V[(chip8->opcode & 0x00F0) >> 4]) {
                chip8->V[0xF] = 1;
            }
            chip8->V[(chip8->opcode & 0x0F00) >> 8] -= chip8->V[(chip8->opcode & 0x00F0) >> 4];
            break;
        case 6:
            if (chip8->V[(chip8->opcode & 0x0F00) >> 8] & 0x1) {
                chip8->V[0xF] = 1;
            } else {
                chip8->V[0xF] = 0;
            }
            break; 
        case 7:
            if (chip8->V[(chip8->opcode & 0x00F0) >> 4] > chip8->V[(chip8->opcode & 0x0F00) >> 8]) {
                chip8->V[0xF] = 1;
            } else {
                chip8->V[0xF] = 0;
            }
            chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x00F0) >> 4] - chip8->V[chip8->opcode << 4 >> 12];
            break;
        case 0xE:
            if (chip8->V[(chip8->opcode & 0x0F00) >> 8] & 0b10000000) {
                chip8->V[0xF] = 1;
            } else {
                chip8->V[(chip8->opcode & 0x0F00) >> 8] *= 2;
            }
            break;
    }
}

void code9(Chip8 *chip8) {
    if (chip8->V[(chip8->opcode & 0x0F00) >> 8] != chip8->V[(chip8->opcode & 0x00F0) >> 4]) {
        chip8->pc += 2;
    }
}

void codeA(Chip8 *chip8) {
    chip8->I = chip8->opcode & 0xFFF;
}

void codeB(Chip8 *chip8) {
    chip8->pc = (chip8->opcode & 0xFFF) + chip8->V[0];
}

void codeC(Chip8 *chip8) {
    uint8_t random_number = rand() % 256;
    chip8->V[(chip8->opcode & 0x0F00) >> 8] = random_number & (chip8->opcode & 0xFF);
}

void codeD(Chip8 *chip8) {
    int n = chip8->opcode & 0x000F;
    int x = chip8->V[(chip8->opcode & 0x0F00) >> 8];
    int y = chip8->V[(chip8->opcode & 0x00F0) >> 4];

    bool flag = false; // If at least one pixel was erased.
    for (size_t i_y = 0; i_y < n; ++i_y) {
        for (size_t i_x = 0; i_x < 8; ++i_x) {
            uint8_t sprite_byte = chip8->memory[chip8->I + i_y];
            sprite_byte = sprite_byte & (1 << (7 - i_x));
            printf("drawing pixel at x: %i and y: %i, where initial x is %i and initial y is %i, and have %i bytes to read\n", x + i_x, y + i_y, x, y, n);
            if (chip8_xor_point(chip8, x + i_x, y + i_y, sprite_byte))
                flag = true;
        }
    }

    chip8->V[0xF] = flag;
}

void codeE(Chip8 *chip8) {
    if (chip8->opcode & 0xFF == 0x9E) {
        if (kbhit == chip8->V[(chip8->opcode & 0x0F00) >> 8]) {
            chip8->pc += 2;
        }
    } else {
        if (kbhit != chip8->V[(chip8->opcode & 0x0F00) >> 8]) {
            chip8->pc += 2;
        }
    }
}

void codeF(Chip8 *chip8) {
    switch (chip8->opcode & 0xFF) {
        case 0x07:
            chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->delay_timer;
            break;
        case 0x0A:
            int temp = getchar();
            chip8->V[(chip8->opcode & 0x0F00) >> 8] = temp;
            break;
        case 0x15:
            chip8->delay_timer = chip8->V[(chip8->opcode & 0x0F00) >> 8];
            break;
        case 0x18:
            chip8->sound_timer = chip8->V[(chip8->opcode & 0x0F00) >> 8];
            break;
        case 0x1E:
            chip8->I += chip8->V[(chip8->opcode & 0x0F00) >> 8];
            break;
        case 0x29:
            chip8->I = chip8->V[(chip8->opcode & 0x0F00) >> 8] * 5;
            break;
        case 0x33:
            uint8_t v = chip8->V[(chip8->opcode & 0x0F00) >> 8];
            for (size_t i = 2; i >= 0; --i) {
                chip8->memory[chip8->I + i] = v % 10;
                v /= 10;
            }
            break;
        case 0x55:
            for (size_t i = 0; i <= ((chip8->opcode & 0x0F00) >> 8); ++i) {
                chip8->memory[chip8->I + i] = chip8->V[i];
            }
            break;
        case 0x65:
            for (size_t i = 0; i <= ((chip8->opcode & 0x0F00) >> 8); ++i) {
                chip8->V[i] = chip8->memory[chip8->I + i];
            }
            break;
    }
}


