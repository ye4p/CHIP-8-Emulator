#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t chip8_font[80] = {
    // 0
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    // 1
    0x20, 0x60, 0x20, 0x20, 0x70,
    // 2
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    // 3
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    // 4
    0x90, 0x90, 0xF0, 0x10, 0x10,
    // 5
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    // 6
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    // 7
    0xF0, 0x10, 0x20, 0x40, 0x40,
    // 8
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    // 9
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    // A
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    // B
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    // C
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    // D
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    // E
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    // F
    0xF0, 0x80, 0xF0, 0x80, 0x80,
};

void chip8_init(Chip8 *chip8, SDL_Renderer* sdl_ptr) {
    chip8->pc = 0x200;
    chip8->opcode = 0;
    chip8->I = 0;
    chip8->sp = 0;
    
    for (size_t i = 0; i < 80; ++i) {
        chip8->memory[i] = chip8_font[i];
    }
    // mempcy(memory, chip8_font, 80); I think these are equivalent
    
    memset(&chip8->framebuffer, 0, sizeof(uint8_t) * 64 * 32);
    memset(&chip8->key, 0, sizeof(uint8_t) * 16);

    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    chip8->rendered_ptr = sdl_ptr;
}

bool chip8_load_rom(Chip8 *chip8, const char *path) {
    FILE *file_ptr = fopen(path, "rb");
    if (file_ptr == NULL) {
        perror("Error opening ROM file");
        return false;
    }

    fseek(file_ptr, 0L, SEEK_END);

    uint64_t file_size = ftell(file_ptr);

    rewind(file_ptr);


    // for (size_t i = 0; i < file_size; ++i ) {
    //     chip8->memory[i + 0x200] =  file_ptr[i];
    // }
    
    size_t read = fread(chip8->memory[0x200], 1, file_size, file_ptr);

    fclose(file_ptr);
    return true;
}

void chip8_emulate_cycle(Chip8 *chip8) {
    // Fetch opcode
    chip8->opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];

    // Decode opcode
    jump_table[chip8->opcode >> 12](chip8, chip8->opcode);

}

void chip8_render(Chip8 *chip8) {
    for (size_t i = 0; i < (64 * 32); ++i) {
        int x = i % 64;
        int y = i / 64;
        if (chip8->framebuffer[i])
            SDL_RenderPoint(chip8->renderer_ptr, (float) x, (float) y);
    }
}
