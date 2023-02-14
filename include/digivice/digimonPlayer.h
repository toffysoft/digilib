#ifndef DIGIMON_PLAYER_H
#define DIGIMON_PLAYER_H

#include "digihal.h"
#include "digimon.h"
#include "digitype.h"

#include "animation.h"

#define AVATAR_STATE_WAITING_HATCH 0
#define AVATAR_STATE_HATCHING      1
#define AVATAR_STATE_WALKING       2
#define AVATAR_STATE_EVOLVING      3

typedef struct avatar_t {
    playing_digimon_t* pstPlayingDigimon;
    animation_t stAnimation;
    uint8_t x, uiFlip, uiCountLoopHatching;
    uint8_t uiState;
    uint16_t uiHoldTime, uiLastIndex;
} avatar_t;

uint8_t DIGIVICE_initAvatar(const digihal_t* pstHal, avatar_t* pstAvatar);

uint8_t DIGIVICE_updateAvatar(avatar_t* pstAvatar, uint16_t uiDeltaTime);

void DIGIVICE_drawAvatar(const avatar_t* pstAvatar);

#endif  // DIGIMON_PLAYER_H