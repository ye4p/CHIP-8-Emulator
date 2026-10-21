#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "instructions.h"
#include <SDL3/SDL_render.h>

#ifndef CHIP8_H
#define CHIP8_H

#define CHIP8_MEMORY_SIZE 4096

static char kbhit = ' ';

typedef struct Chip8 {
    uint16_t opcode;

    uint8_t memory[CHIP8_MEMORY_SIZE];

    uint8_t V[16];  // General purpose 16 registers (VF shouldn't be used for the program as used as a flag by some instructions)
    uint16_t I;     // Generally used to store memory addresses
    uint8_t delay_timer;    
    uint8_t sound_timer;   
    uint16_t pc;    // Program Counter
    uint8_t sp;     // Stack Pointer
    uint16_t stack[16];


    uint8_t framebuffer[64 * 32];   // For displaying
    
    uint8_t key[16];

    void *renderer_ptr;
} Chip8;

void chip8_init(Chip8* chip8, SDL_Renderer* sdl_ptr);
bool chip8_load_rom(Chip8* chip8, const char* path);
void chip8_emulate_cycle(Chip8* chip8);
void chip8_update_timers(Chip8 *chip8);
void chip8_set_key(Chip8 *chip8, uint8_t key, bool pressed);

void chip8_render(Chip8 *chip8);


#endif
