/* points.c ... */
#include "sdl.h"

/*
 * This example creates an SDL window and renderer, and then draws some points
 * to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Uint64 last_time = 0;

/* (track everything as parallel arrays instead of a array of structs,
   so we can pass the coordinates to the renderer in a single function call.) */

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate)
{
    int i;

    SDL_SetAppMetadata("CHIP-8 Emulator", "1.0", "com.example.renderer-points");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/points", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    last_time = SDL_GetTicks();

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate, uint8_t* framebuffer)
{
    const Uint64 now = SDL_GetTicks();
    const float elapsed = ((float) (now - last_time)) / 1000.0f;  /* seconds since last iteration */
    last_time = now;

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    // SDL_RenderPoints(renderer, points, SDL_arraysize(points));  /* draw all the points! */

    /* You can also draw single points with SDL_RenderPoint(), but it's
       cheaper (sometimes significantly so) to do them all at once. */

    for (size_t i = 0; i < (64 * 32); ++i) {
        int x = i % 64;
        int y = i / 64;
        if (framebuffer[i]) {
            SDL_RenderPoint(renderer, (float) x, (float) y);
            printf("Rendering point at %i and %i\n", x, y);
        }
    }

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}

