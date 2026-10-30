#include "chip8.h"

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

void chip8_init(Chip8 *chip8) {
    chip8->pc = 0x200;
    chip8->opcode = 0;
    chip8->I = 0;
    chip8->sp = 0;
   
    // memset(chip8->memory, 0, 4096);

    // for (size_t i = 0; i < 80; ++i) {
    //     chip8->memory[i] = chip8_font[i];
    // }
    
    memcpy(chip8->memory, chip8_font, 80);    

    memset(&chip8->framebuffer, 0, sizeof(uint8_t) * 64 * 32);
    memset(&chip8->key, 0, sizeof(uint8_t) * 16);

    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    chip8->counter = 0;
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

    size_t read = fread(chip8->memory + 0x200, 1, file_size, file_ptr);

    fclose(file_ptr);

    printf("ROM loaded successfully\n");
    return true;
}

void chip8_emulate_cycle(Chip8 *chip8) {
    // Fetch opcode
    chip8->opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];
    chip8->pc += 2;

    printf("Trying to execute instruction instruction %x %i %i %i\n", (chip8->opcode & 0xF000) >> 12, (chip8->opcode & 0x0F00) >> 8, (chip8->opcode & 0x00F0) >> 4, chip8->opcode & 0x000F);
    // Decode opcode and execute it
    jump_table[chip8->opcode >> 12](chip8);

    printf("instruc %x %x %x %x\n", (chip8->opcode & 0xF000) >> 12, (chip8->opcode & 0x0F00) >> 8, (chip8->opcode & 0x00F0) >> 4, chip8->opcode & 0x000F);

    chip8_update_timers(chip8);
}

bool chip8_xor_point(Chip8 *chip8, int x, int y, uint8_t sprite_pixel) {
    while (x > 63) {
        x -= 64;
    }
    while (x < 0) {
        x += 64;
    }

    while (y > 31) {
        y -= 32;
    }
    while (y < 0) {
        y += 31;
    }
    // printf("ended up drawing at x: %i and y: %i\n", x, y);
    int sq = y * 64 + x;
    if (sq >= (64 * 32) || sq < 0) {
        fprintf(stderr, "trying to draw a pixel out of bounds: x=%i, y=%i\n", x, y);
        exit(EXIT_FAILURE);
    }
    bool old = chip8->framebuffer[sq];
    chip8->framebuffer[sq] = (!chip8->framebuffer[sq]) != (!sprite_pixel);

    return old && !chip8->framebuffer[sq];
}

void chip8_update_timers(Chip8 *chip8) {
    if ((chip8->counter++ % 12) == 0) {
        if (chip8->delay_timer) --chip8->delay_timer;
        if (chip8->sound_timer) --chip8->sound_timer;
    }
}

int chip8_key_to_index(char c) {
    int pressed;

    if (c >= 48 && c <= 57) { // It's a digit
        pressed = c - '0';
        return pressed;
    }

    if (c >= 97 && c <= 102) { // It's a character from A to F
        pressed = (c - 'a') + 10; 
        return pressed;
    }   

    pressed = -1;
    return pressed;
}
char chip8_index_to_key(int n) {
    if (n < 0 || n >= 16) {
        fprintf(stderr, "trying to get key that isn't hex: %i", n);
        exit(EXIT_FAILURE);
    }

    if (n < 10) {
        return (char)n;
    }

    // Return letter char:
    return (char)(n - 10 + 97);
}
