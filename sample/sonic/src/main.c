#include <genesis.h>

#include "player.h"
#include "entities.h"
#include "hud.h"
#include "level.h"
#include "camera.h"
#include "sfx.h"

#include "res_gfx.h"
#include "res_sound.h"


bool paused;

// forward
static void handleInput();
static void joyEvent(u16 joy, u16 changed, u16 state);
static void vblank();


<<<<<<< HEAD
// 42 * 32 = complete tilemap update; * 2 as we have 2 full plans to update potentially
// used for alternate map update mode
u16 tilemapBuf[42 * 32 * 2];
u16 bufOffset;

// player (sonic) sprite
Sprite* player;
// enemies sprites
Sprite* enemies[2];

// Speed, Jump and Gravity interface
Sprite* bars[3];

// maps (BGA and BGB) position (tile) for alternate method
s16 mapMetaTilePosX[2];
s16 mapMetaTilePosY[2];
// maps (BGA and BGB)
Map *bgb;
Map *bga;

// absolute camera position (pixel)
s16 camPosX;
s16 camPosY;
// require scroll update
bool scrollNeedUpdate;

// physic variables
fix32 maxSpeed;
fix32 jumpSpeed;
fix32 gravity;

// position and movement variables
fix32 posX;
fix32 posY;
fix32 movX;
fix32 movY;
s16 xOrder;
s16 yOrder;

// enemies positions and move direction
fix32 enemiesPosX[2];
fix32 enemiesPosY[2];
s16 enemiesXOrder[2];

// animation index table for enemies (static VRAM loading)
u16** sprTileIndexes[2];
// BG start tile index
u16 bgBaseTileIndex[2];

// maintain X button to use alternate MAP update mode
bool alternateScrollMethod;
bool paused;

int main(bool hard)
{
    u16 palette[64];
    u16 ind;

    paused = FALSE;

    // initialization
    VDP_setScreenWidth320();
    // set all palette to black
    PAL_setColors(0, (u16*) palette_black, 64, CPU);

    // init SFX
    SFX_init();
    // start music
    XGM_startPlay(sonic_music);

    // need to increase a bit DMA buffer size to init both plan tilemap and sprites
    DMA_setBufferSize(10000);
    DMA_setMaxTransferSize(10000);

    // init sprite engine with default parameters
    SPR_init();

    ind = TILE_USER_INDEX;
    ind += LEVEL_init(ind);
    CAMERA_init();
    ind += PLAYER_init(ind);
    ind += ENTITIES_init(ind);
    ind += HUD_init(ind);

    // set camera position
    CAMERA_centerOn(160, 100);
    // update sprite
    SPR_update();
    // and init map
    SYS_doVBlankProcess();

    // can restore default DMA buffer size
    DMA_setBufferSizeToDefault();
    DMA_setMaxTransferSizeToDefault();

    // prepare palettes (BGB image contains the 4 palettes data)
    memcpy(&palette[0], palette_all.data, 64 * 2);
//    memcpy(&palette[16], bga_image.palette->data, 16 * 2);
//    memcpy(&palette[32], sonic_sprite.palette->data, 16 * 2);
//    memcpy(&palette[48], enemies_sprite.palette->data, 16 * 2);

    // fade in
    PAL_fadeIn(0, (4 * 16) - 1, palette, 20, TRUE);

    // set joy and vblank handler
    JOY_setEventHandler(joyEvent);
    SYS_setVBlankCallback(vblank);

    // just to monitor frame CPU usage
    SYS_showFrameLoad(TRUE);

//    reseted = FALSE;

    while (TRUE)
    {
        // first
        handleInput();

        if (!paused)
        {
            // update player first
            PLAYER_update();
            // then set camera from player position
            CAMERA_centerOn(fix32ToInt(posX), fix32ToInt(posY));

            // then we can update entities
            ENTITIES_update();

            // better to do it separately, when camera position is up to date
            PLAYER_updateScreenPosition();
            ENTITIES_updateScreenPosition();
        }

        // update sprites
        SPR_update();

        // sync frame and do vblank process
        SYS_doVBlankProcess();
    }

    // release maps
    MEM_free(bga);
    MEM_free(bgb);

    return 0;
}

static void updateBarsVisitility()
{
    if (paused)
    {
        SPR_setVisibility(bars[0], VISIBLE);
        SPR_setVisibility(bars[1], VISIBLE);
        SPR_setVisibility(bars[2], VISIBLE);
    }
    else
    {
        SPR_setVisibility(bars[0], HIDDEN);
        SPR_setVisibility(bars[1], HIDDEN);
        SPR_setVisibility(bars[2], HIDDEN);
    }
}

static void updateBar(Sprite* bar, f32 min, f32 max, f32 current)
{
    f32 levelf;
    s16 leveli;

    levelf = fix32Mul(current, FIX32(16));
    levelf = fix32Div(levelf, (max - min));
    levelf -= min;

    leveli = fix32ToInt(levelf);
    if (leveli < 0) leveli = 0;
    else if (leveli > 16) leveli = 16;

    SPR_setFrame(bar, leveli);
}

static void updatePhysic()
{
    u16 i;

    // sonic physic
    if (xOrder > 0)
    {
        movX += ACCEL;
        // going opposite side, quick breaking
        if (movX < 0) movX += ACCEL;

        if (movX >= maxSpeed) movX = maxSpeed;
    }
    else if (xOrder < 0)
    {
        movX -= ACCEL;
        // going opposite side, quick breaking
        if (movX > 0) movX -= ACCEL;

        if (movX <= -maxSpeed) movX = -maxSpeed;
    }
    else
    {
        if ((movX < FIX32(0.1)) && (movX > FIX32(-0.1)))
            movX = 0;
        else if ((movX < FIX32(0.3)) && (movX > FIX32(-0.3)))
            movX -= movX >> 2;
        else if ((movX < FIX32(1)) && (movX > FIX32(-1)))
            movX -= movX >> 3;
        else
            movX -= movX >> 4;
    }

    posX += movX;
    posY += movY;

    if (movY)
    {
        if (posY > MAX_POSY)
        {
            posY = MAX_POSY;
            movY = 0;
        }
        else movY += gravity;
    }

    if (posX >= MAX_POSX)
    {
        posX = MAX_POSX;
        movX = 0;
    }
    else if (posX <= MIN_POSX)
    {
        posX = MIN_POSX;
        movX = 0;
    }

    // enemies physic
    if (enemiesXOrder[0] > 0) enemiesPosX[0] += FIX32(1.5);
    else enemiesPosX[0] -= FIX32(1.5);
    if (enemiesXOrder[1] > 0) enemiesPosX[1] += FIX32(0.7);
    else enemiesPosX[1] -= FIX32(0.7);
    for (i = 0; i < 2; i++)
    {
        if ((enemiesPosX[i] >= MAX_POSX) || (enemiesPosX[i] <= MIN_POSX))
            enemiesXOrder[i] = -enemiesXOrder[i];
    }

    // update camera position (*after* player sprite position has been updated)
    updateCameraPosition();

    // set sprites position
    setSpritePosition(player, fix32ToInt(posX) - camPosX, fix32ToInt(posY) - camPosY);
    setSpritePosition(enemies[0], fix32ToInt(enemiesPosX[0]) - camPosX, fix32ToInt(enemiesPosY[0]) - camPosY);
    setSpritePosition(enemies[1], fix32ToInt(enemiesPosX[1]) - camPosX, fix32ToInt(enemiesPosY[1]) - camPosY);
}

static void setSpritePosition(Sprite* sprite, s16 x, s16 y)
{
    // clip out of screen sprites
    if ((x < -100) || (x > 320) || (y < -100) || (y > 240)) SPR_setVisibility(sprite, HIDDEN);
    else
    {
        SPR_setVisibility(sprite, VISIBLE);
        SPR_setPosition(sprite, x, y);
    }
}

static void updateAnim()
{
    // jumping
    if (movY) SPR_setAnim(player, ANIM_ROLL);
    else
    {
        if (((movX >= BRAKE_SPEED) && (xOrder < 0)) || ((movX <= -BRAKE_SPEED) && (xOrder > 0)))
        {
            if (player->animInd != ANIM_BRAKE)
            {
                SND_startPlayPCM_XGM(SFX_STOP, 1, SOUND_PCM_CH2);
                SPR_setAnim(player, ANIM_BRAKE);
            }
        }
        else if ((movX >= RUN_SPEED) || (movX <= -RUN_SPEED))
            SPR_setAnim(player, ANIM_RUN);
        else if (movX != 0)
            SPR_setAnim(player, ANIM_WALK);
        else
        {
            if (yOrder < 0)
                SPR_setAnim(player, ANIM_UP);
            else if (yOrder > 0)
                SPR_setAnim(player, ANIM_CROUNCH);
            else
                SPR_setAnim(player, ANIM_STAND);
        }
    }

    if (movX > 0) SPR_setHFlip(player, FALSE);
    else if (movX < 0) SPR_setHFlip(player, TRUE);

    // enemies
    if (enemiesXOrder[0] > 0) SPR_setHFlip(enemies[0], TRUE);
    else SPR_setHFlip(enemies[0], FALSE);
//    for(i = 0; i < 2; i++)
//    {
//        if (enemiesXOrder[i] > 0) SPR_setHFlip(sprites[i + 1], TRUE);
//        else SPR_setHFlip(sprites[i + 1], FALSE);
//    }
}

static void updateCameraPosition()
{
    // get player position (pixel)
    s16 px = fix32ToInt(posX);
    s16 py = fix32ToInt(posY);
    // current sprite position on screen
    s16 px_scr = px - camPosX;
    s16 py_scr = py - camPosY;

    s16 npx_cam, npy_cam;

    // adjust new camera position
    if (px_scr > 240) npx_cam = px - 240;
    else if (px_scr < 40) npx_cam = px - 40;
    else npx_cam = camPosX;
    if (py_scr > 140) npy_cam = py - 140;
    else if (py_scr < 60) npy_cam = py - 60;
    else npy_cam = camPosY;

    // clip camera position
    if (npx_cam < 0) npx_cam = 0;
    else if (npx_cam > (MAP_WIDTH - 320)) npx_cam = (MAP_WIDTH - 320);
    if (npy_cam < 0) npy_cam = 0;
    else if (npy_cam > (MAP_HEIGHT - 224)) npy_cam = (MAP_HEIGHT - 224);

    // set new camera position
    setCameraPosition(npx_cam, npy_cam);
}

static void setCameraPosition(s16 x, s16 y)
{
    if ((x != camPosX) || (y != camPosY))
    {
        camPosX = x;
        camPosY = y;

        // alternate map update method ?
        if (alternateScrollMethod)
        {
            // update maps (convert pixel to metatile coordinate)
            updateMap(BG_A, bga, x >> 4, y >> 4);
            // scrolling is slower on BGB, no vertical scroll (should be consisten with updateVDPScroll())
            updateMap(BG_B, bgb, x >> 7, y >> 9);

            // request VDP scroll update
            scrollNeedUpdate = TRUE;
        }
        else
        {
            // scroll maps
            MAP_scrollTo(bga, x, y);
            // scrolling is slower on BGB
            MAP_scrollTo(bgb, x >> 3, y >> 5);
        }

        // always store it to avoid full map update on method change
        mapMetaTilePosX[BG_A] = x >> 4;
        mapMetaTilePosY[BG_A] = y >> 4;
        mapMetaTilePosX[BG_B] = x >> 7;
        mapMetaTilePosY[BG_B] = y >> 9;
    }
}

// this is just to show how use the MAP_getTilemapRect(..) method
// if we weed to actually access tilemap data and do manual tilemap update to VDP
static void updateMap(VDPPlane plane, Map* map, s16 xmt, s16 ymt)
{
    // BGA = 0; BGB = 1
    s16 cxmt = mapMetaTilePosX[plane];
    s16 cymt = mapMetaTilePosY[plane];
    s16 deltaX = xmt - cxmt;
    s16 deltaY = ymt - cymt;

    // no update --> exit
    if ((deltaX == 0) && (deltaY == 0)) return;

    // clip to 21 metatiles column max (full screen update)
    if (deltaX > 21)
    {
        cxmt += deltaX - 21;
        deltaX = 21;
        deltaY = 0;
    }
    // clip to 21 metatiles column max (full screen update)
    else if (deltaX < -21)
    {
        cxmt += deltaX + 21;
        deltaX = -21;
        deltaY = 0;
    }
    // clip to 16 metatiles row max (full screen update)
    else if (deltaY > 16)
    {
        cymt += deltaY - 16;
        deltaY = 16;
        deltaX = 0;
    }
    // clip to 16 metatiles row max (full screen update)
    else if (deltaY < -16)
    {
        cymt += deltaY + 16;
        deltaY = -16;
        deltaX = 0;
    }

    if (deltaX > 0)
    {
        // update on right
        cxmt += 21;

        // need to update map column on right
        while (deltaX--)
        {
            MAP_getTilemapRect(map, cxmt, ymt, 1, 16, TRUE, tilemapBuf + bufOffset);
            VDP_setTileMapDataColumnFast(plane, tilemapBuf + bufOffset, (cxmt * 2) + 0, ymt * 2, 16 * 2, DMA_QUEUE);
            // next column
            bufOffset += 16 * 2;
            VDP_setTileMapDataColumnFast(plane, tilemapBuf + bufOffset, (cxmt * 2) + 1, ymt * 2, 16 * 2, DMA_QUEUE);
            // next column
            bufOffset += 16 * 2;
            cxmt++;
        }
    }
    else
    {
        // need to update map column on left
        while (deltaX++)
        {
            cxmt--;
            MAP_getTilemapRect(map, cxmt, ymt, 1, 16, TRUE, tilemapBuf + bufOffset);
            VDP_setTileMapDataColumnFast(plane, tilemapBuf + bufOffset, (cxmt * 2) + 0, ymt * 2, 16 * 2, DMA_QUEUE);
            // next column
            bufOffset += 16 * 2;
            VDP_setTileMapDataColumnFast(plane, tilemapBuf + bufOffset, (cxmt * 2) + 1, ymt * 2, 16 * 2, DMA_QUEUE);
            // next column
            bufOffset += 16 * 2;
        }
    }

    if (deltaY > 0)
    {
        // update at bottom
        cymt += 16;

        // need to update map row on bottom
        while (deltaY--)
        {
            MAP_getTilemapRect(map, xmt, cymt, 21, 1, FALSE, tilemapBuf + bufOffset);
            VDP_setTileMapDataRow(plane, tilemapBuf + bufOffset, (cymt * 2) + 0, (xmt * 2), 21 * 2, DMA_QUEUE);
            // next row
            bufOffset += 21 * 2;
            VDP_setTileMapDataRow(plane, tilemapBuf + bufOffset, (cymt * 2) + 1, (xmt * 2), 21 * 2, DMA_QUEUE);
            // next row
            bufOffset += 21 * 2;
            cymt++;
        }
    }
    else
    {
        // need to update map row on top
        while (deltaY++)
        {
            cymt--;
            MAP_getTilemapRect(map, xmt, cymt, 21, 1, FALSE, tilemapBuf + bufOffset);
            VDP_setTileMapDataRow(plane, tilemapBuf + bufOffset, (cymt * 2) + 0, (xmt * 2), 21 * 2, DMA_QUEUE);
            // next row
            bufOffset += 21 * 2;
            VDP_setTileMapDataRow(plane, tilemapBuf + bufOffset, (cymt * 2) + 1, (xmt * 2), 21 * 2, DMA_QUEUE);
            // next row
            bufOffset += 21 * 2;
        }
    }

    mapMetaTilePosX[plane] = xmt;
    mapMetaTilePosY[plane] = ymt;
}

static void updateVDPScroll()
{
    VDP_setHorizontalScroll(BG_A, -camPosX);
    VDP_setHorizontalScroll(BG_B, (-camPosX) >> 3);
    VDP_setVerticalScroll(BG_A, camPosY);
    VDP_setVerticalScroll(BG_B, camPosY >> 5);
}

static void frameChanged(Sprite* sprite)
{
    // get enemy index (stored in data field)
    u16 enemyIndex = sprite->data;
    // get VRAM tile index for this animation of this sprite
    u16 tileIndex = sprTileIndexes[enemyIndex][sprite->animInd][sprite->frameInd];
    // manually set tile index for the current frame (preloaded in VRAM)
    SPR_setVRAMTileIndex(sprite, tileIndex);
}

static void handleInput()
{
    u16 value = JOY_readJoypad(JOY_1);

    // game is paused ?
    if (paused)
    {
        // adjust physics settings
        HUD_handleInput(value);
    }
    else
    {
        // can affect gameplay
        PLAYER_handleInput(value);
        CAMERA_handleInput(value);
    }
}

static void joyEvent(u16 joy, u16 changed, u16 state)
{
    // START button state changed --> pause / unpause
    if (changed & state & BUTTON_START)
    {
        paused = !paused;
        HUD_setVisibility(paused);
    }

    // can't do more in paused state
    if (paused) return;

    // handle player joy actions
    PLAYER_doJoyAction(joy, changed, state);
}

static void vblank()
{
    // handle vblank stuff
    LEVEL_onVBlank();
}