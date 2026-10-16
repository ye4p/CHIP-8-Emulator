#include <stdio.h>
#include <stdint.h>
#include "src/chip8.h"
#include "graphics/sdl.h"

int main() {
    Chip8 chip8;
    void *ptr = NULL;
    printf("loading the window...\n");
    
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
            if (SDL_AppIterate(ptr) != SDL_APP_CONTINUE) {
                running = false;
            }
        }
        
    }

    SDL_AppQuit((ptr), SDL_APP_SUCCESS);
    return 0;
}
