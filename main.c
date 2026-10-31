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
        "../games/br8kout.ch8",
        "../games/flightrunner.ch8",
        "../games/Tetris [Fran Dachille, 1991].ch8",
        "../games/Shooting Stars [Philip Baltzer, 1978].ch8",
        "../games/Animal Race [Brian Astle].ch8",
    };
    uint64_t program_count = sizeof(programs) / sizeof(char*);

    printf("List of available programs to run(0-8 are tests, 9+ are games): \n\n");
    
    printf("Programs:\n");
    for (size_t i = 0; i < program_count; ++i) {
        printf("Program %u: %s,\n", i + 1, programs[i]);
    }

    printf("\nEnter number(1-%u):\n", program_count);
    
    char word[10];
    char *endptr;
    long program_number;
    scanf("%10s", word);
    program_number = strtol(word, &endptr, 10) - 1;

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

    unsigned clear_count = 0;
    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (SDL_AppEvent(ptr, &event) != SDL_APP_CONTINUE) {
                running = false;
                break;
            }

            if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                int index = chip8_key_to_index((char)event.key.key);

                if (index != -1) {
                    bool val = (event.type == SDL_EVENT_KEY_DOWN);
                    chip8.key[index] = val;
                    const char* where = val ? "down" : "up";
                    printf("CHARACTER %c FROM INDEX %i WAS PRESSED %s\n", (char)event.key.key, index, where);
                }
            }

            if (event.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
                for(size_t i = 0; i < 16; ++i) {
                    chip8.key[i] = 0;
                }
            }

            if (event.type == SDL_EVENT_KEY_DOWN) 
                break;
        }

        if (running) {
            int count = 12;
            while (count--) {
                chip8_emulate_cycle(&chip8);
            }
            printf("%i instructions got executed\n", count);

            if (SDL_AppIterate(ptr, chip8.framebuffer) != SDL_APP_CONTINUE) {
                running = false;
            }

            
        }

        if ((clear_count++ % 5) == 0) {
            // Clear key array.
            for (size_t i = 0; i < 16; ++i) 
                chip8.key[i] = 0;
        }

        thrd_sleep(&duration, NULL);

        
    }

    SDL_AppQuit((ptr), SDL_APP_SUCCESS);
    return 0;
}
