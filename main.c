#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "src/chip8.h"
#include "graphics/sdl.h"

int main() {
    srand(time(NULL));
    Chip8 chip8;
    void *ptr = NULL;
    printf("loading the window...\n");

    chip8_init(&chip8);
    
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
            for (size_t i = 0; i < 1500; ++i) {
                if (i % 2 == 0) continue;
                chip8.framebuffer[i] = 1;
            } 
            if (SDL_AppIterate(ptr, chip8.framebuffer) != SDL_APP_CONTINUE) {
                running = false;
            }

        }
        
    }

    SDL_AppQuit((ptr), SDL_APP_SUCCESS);
    return 0;
}
