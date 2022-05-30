#include "digivice/game.h"
#include "SDL2/SDL.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int initGame() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Digivice", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    if (window == NULL) {
        SDL_Log("Failed creating window");
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Failed creating renderer from window");
        cleanUpGame();
        return 0;
    }

    SDL_Log("Initialized");
    return 1;
}

int updateGame() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            SDL_Log("Closing game");
            return 0;
        }
    }

    // TODO: Game Logic.

    return 1;
}

void drawGame() {
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(1000 / 60);  // 60 fps
}

void cleanUpGame() {
    if (renderer) {
        SDL_Log("Destroying renderer");
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_Log("Destroying window");
        SDL_DestroyWindow(window);
    }

    SDL_Log("Qutting SDL");
    SDL_Quit();
}