#include "digivice.h"

#include "enums.h"

#include "digimonPlayer.h"
#include "render.h"

static size_t guiFrequency;

static avatar_t gstPlayer;

uint8_t DIGIVICE_init(const digihal_t* pstHal,
                      const digivice_hal_t* pstDigiviceHal,
                      size_t uiFrequency) {
    uint8_t uiRet = DIGIVICE_initAvatar(pstHal, &gstPlayer);

    gpstDigiviceHal = pstDigiviceHal;
    guiFrequency = uiFrequency;
    return uiRet;
}

static uint32_t getDeltaTime() {
    static size_t uiLastTime = 0;
    size_t uiCurrentTime = gpstDigiviceHal->getTimeStamp();
    if (uiLastTime == 0)
        uiLastTime = uiCurrentTime;
    uint32_t uiDeltaTime = uiCurrentTime - uiLastTime;
    uiLastTime = uiCurrentTime;
    return uiDeltaTime;
}

uint8_t DIGIVICE_update() {
    uint32_t uiDeltaTime = getDeltaTime();
    uint8_t uiRet = DIGI_RET_OK;

    DIGIVICE_updateAvatar(&gstPlayer, uiDeltaTime);

    DIGIVICE_drawAvatar(&gstPlayer);
    gpstDigiviceHal->render();
    return uiRet;
}
