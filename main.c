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
    const char* program_path = "../tests/1-chip8-logo.ch8";
    struct timespec duration;
    duration.tv_sec = 0;
    // duration.tv_nsec = 500000000;
    duration.tv_nsec = 16000000;

    chip8_init(&chip8);
    bool is_loaded = chip8_load_rom(&chip8, program_path);
   
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
