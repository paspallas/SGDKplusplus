#include <genesis.h>
#include <cstring>
#include <bitset>

#include <sprite.h>
#include "JoyPad.hpp"
#include "Ramiro.hpp"
#include "Vector2F.hpp"

using ComponentType = uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

int main(bool hardReset)
{
    SYS_disableInts();

    VDP_setEnable(false);
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    PAL_setPalette(PAL0, palette_black, CPU);
    VDP_setTextPalette(PAL3);
    VDP_setEnable(true);

    SYS_enableInts();

    u16 palette[64];
    std::memcpy(&palette[32], spr_ramiro.palette->data, 16 * 2);
    PAL_fadeIn(0, 64, palette, 20, false);

    SPR_init();

    auto sprite = SPR_addSprite(&spr_ramiro, 10, 10, TILE_ATTR(PAL2, true, false, true));
    Ramiro player{ sprite, FIX16(10), FIX16(10) };

    while (true)
    {
        JoyPad::GetInstance().Update();

        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}
