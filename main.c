#include <stdio.h>
#include <stdint.h>

typedef struct Chip8 {
    uint8_t V[16],  // General purpose 16 registers (VF shouldn't be used for the program as used as a flag by some instructions)
    uint16_t I,     // Generally used to store memory addresses
    uint8_t DT,     // Delay Timer
    uint8_t ST,     // Sound Timer
    uint16_t PC,    // Program Counter
    uint8_t SP,     // Stack Pointer
    uint16_t stack[16],

    uint8_t memory[Kilobytes(4)],

    uint8_t framebuffer[64 * 32],   // For displaying
} Chip8;

int main() {
    printf("Hello, World!");
    return 0;
}
