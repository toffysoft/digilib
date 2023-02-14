#include "digimonPlayer.h"

#include "digiapi.h"
#include "enums.h"

#include "render.h"
#include "sprites.h"

#define CENTER_SCREEN 8

uint8_t DIGIVICE_initAvatar(const digihal_t* pstHal, avatar_t* pstAvatar) {
    uint8_t uiRet = DIGI_init(pstHal, &pstAvatar->pstPlayingDigimon);
    if (uiRet == DIGI_RET_CHOOSE_DIGITAMA) {
        uiRet = DIGI_selectDigitama(pstAvatar->pstPlayingDigimon, 0);
        pstAvatar->x = 8;
        pstAvatar->uiLastIndex =
            pstAvatar->pstPlayingDigimon->uiIndexCurrentDigimon;
        pstAvatar->stAnimation.puiCurrentAnimation =
            guiDigimonWalkingAnimationDatabase[pstAvatar->pstPlayingDigimon
                                                   ->uiIndexCurrentDigimon];
        pstAvatar->stAnimation.uiMaxFrameCount = 2;
        pstAvatar->uiHoldTime = 500;
    }
    return uiRet;
}

void avatarUpdateState(avatar_t* pstAvatar, uint8_t uiEvents) {
    if (uiEvents & DIGI_EVENT_MASK_EVOLVE) {
        if (pstAvatar->pstPlayingDigimon->pstCurrentDigimon->uiStage ==
            DIGI_STAGE_BABY_1) {
            pstAvatar->uiState = AVATAR_STATE_HATCHING;
            pstAvatar->uiHoldTime = 75;
            pstAvatar->uiCountLoopHatching = 0;
        } else {
            pstAvatar->uiState = AVATAR_STATE_EVOLVING;
        }
    }
}

void avatarUpdatePosition(avatar_t* pstAvatar) {
    switch (pstAvatar->uiState) {
        case AVATAR_STATE_WAITING_HATCH:
            pstAvatar->x = CENTER_SCREEN;
            break;
        case AVATAR_STATE_HATCHING:
            if (pstAvatar->uiCountLoopHatching >= 8) {
                pstAvatar->uiCountLoopHatching++;

                if (pstAvatar->x > CENTER_SCREEN)
                    pstAvatar->x -= 1;
                if (pstAvatar->uiCountLoopHatching >= 24) {
                    pstAvatar->uiLastIndex =
                        pstAvatar->pstPlayingDigimon->uiIndexCurrentDigimon;
                    pstAvatar->uiState = AVATAR_STATE_WALKING;
                }
                break;
            }

            if ((pstAvatar->uiCountLoopHatching & 1) == 0) {
                pstAvatar->x -= 1;
            } else {
                pstAvatar->x += 1;
            }

            if (pstAvatar->x >= 10 || pstAvatar->x <= 6) {
                pstAvatar->uiCountLoopHatching++;
            }
            break;
        case AVATAR_STATE_WALKING:
            break;
        default:
            break;
    }
}

uint8_t DIGIVICE_updateAvatar(avatar_t* pstAvatar, uint16_t uiDeltaTime) {
    static uint16_t uiTimePassedMilliSecond = 0;
    static uint32_t uiTimePassedHalfMinute = 60000;
    uint8_t uiRet, uiEvents;

    uiTimePassedMilliSecond += uiDeltaTime;
    uiTimePassedHalfMinute += uiDeltaTime;
    if (uiTimePassedMilliSecond >= pstAvatar->uiHoldTime) {
        avatarUpdatePosition(pstAvatar);

        uiTimePassedMilliSecond = 0;
    } else {
        DIGIVICE_updateAnimation(&pstAvatar->stAnimation, uiDeltaTime);
    }
    if (uiTimePassedHalfMinute >= 60000) {
        pstAvatar->uiLastIndex =
            pstAvatar->pstPlayingDigimon->uiIndexCurrentDigimon;
        uiRet = DIGI_updateEventsDeltaTime(pstAvatar->pstPlayingDigimon, 1,
                                           &uiEvents);
        avatarUpdateState(pstAvatar, uiEvents);
        uiTimePassedHalfMinute = 0;
    }
    gpstHal->log("Deltatime avatar %d %d", uiTimePassedMilliSecond,
                 uiDeltaTime);
    return uiRet;
}

void DIGIVICE_drawAvatar(const avatar_t* pstAvatar) {
    if (pstAvatar->uiState == AVATAR_STATE_HATCHING) {
        const uint8_t uiCurrentFrame =
            pstAvatar->uiCountLoopHatching >= 12 ? 2 : 0;

        const uint16_t* const pstCurrentAnimation =
            guiDigimonWalkingAnimationDatabase[pstAvatar->uiLastIndex]
                                              [uiCurrentFrame];

        DIGIVICE_drawSprite(pstCurrentAnimation, pstAvatar->x, 0,
                            pstAvatar->uiFlip);
    } else {
        const uint16_t* const pstCurrentAnimation =
            DIGIVICE_getCurrentSpriteAnimation(&pstAvatar->stAnimation);
        DIGIVICE_drawSprite(pstCurrentAnimation, pstAvatar->x, 0,
                            pstAvatar->uiFlip);
    }
}