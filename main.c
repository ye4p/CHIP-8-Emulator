#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <threads.h>
#include "src/chip8.h"
#include "graphics/sdl.h"

int main() {
    srand(time(NULL));
    Chip8 chip8;
    void *ptr = NULL;
    const char* programs[] = {
        "../tests/1-chip8-logo.ch8",
        "../tests/2-ibm-logo.ch8",
        "../tests/3-corax+.ch8",
        "../tests/4-flags.ch8",
        "../tests/5-quirks.ch8",
        "../tests/6-keypad.ch8",
        "../tests/7-beep.ch8",
        "../tests/8-scrolling.ch8",
    };

    printf("List of available programs to run: \n\n");
    
    printf("Tests:\n");
    for (size_t i = 0; i < 8; ++i) {
        printf("Test %i: %s,\n", i + 1, programs[i]);
    }

    printf("\nEnter number(1-8):\n");
    
    char ch = getchar();
    int program_number = ch - '1';
    printf("Launching a program %i: %s\n", program_number, programs[program_number]);

    struct timespec duration;
    duration.tv_sec = 0;
    duration.tv_nsec = 16000000;

    chip8_init(&chip8);
    bool is_loaded = chip8_load_rom(&chip8, programs[program_number]);
   
    if (!is_loaded) 
        return 1;

    if (SDL_AppInit(&ptr) != SDL_APP_CONTINUE) {
        SDL_AppQuit(ptr, SDL_APP_FAILURE);
        return 1;
    }

    bool running = true;

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (SDL_AppEvent(ptr, &event) != SDL_APP_CONTINUE) {
                running = false;
                break;
            }
        }

        if (running) {
            chip8_update_key();

            chip8_emulate_cycle(&chip8);

            // for (size_t i = 0; i < 1500; ++i) {
            //     if (i % 2 == 0) continue;
            //         chip8.framebuffer[i] = 1;
            // } 

            if (SDL_AppIterate(ptr, chip8.framebuffer) != SDL_APP_CONTINUE) {
                running = false;
            }

            thrd_sleep(&duration, NULL);

        }
        
    }

    SDL_AppQuit((ptr), SDL_APP_SUCCESS);
    return 0;
}
