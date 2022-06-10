#ifndef AVATAR_H
#define AVATAR_H

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"

#include "animation.h"
#include "digimon.h"

#define SAVE_FILE "digimon.save"

typedef enum Action {
    HATCHING,
    EVOLVING,
    WALKING,
    EATING,
    SLEEPING,
    HEALING,
    HAPPY,
    NEGATING
} Action;

typedef struct {
    char name[256];

    SDL_Texture* spriteSheet;
    SDL_Rect transform;
    SDL_RendererFlags renderFlags;

    AnimationController animationController;
    Action currentAction;

    float timePassed;
    int secondsPassed;
    int initiated;

    playing_digimon_t infoApi;
} Avatar;

int initAvatar(Avatar* ret);

void updateAvatar(Avatar* avatar, const float deltaTime);

void drawAvatar(SDL_Renderer* render, const Avatar* avatar);

void handleEvents(Avatar* avatar, const unsigned char events);

void freeAvatar(Avatar* avatar);

#endif