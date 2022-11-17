#ifndef _GENESIS_H_
#define _GENESIS_H_

#define SGDK_VERSION 180
#define SGDK_VERSION_MINOR 80
#define SGDK_VERSION_MAJOR 1

#ifdef __cplusplus
extern "C"
{
#endif

#include "types.h"

#define SGDK TRUE

#include "config.h"
#include "asm.h"

#include "sys.h"
#include "sram.h"
#include "mapper.h"
#include "memory.h"
#include "tools.h"

#include "pool.h"
#include "object.h"

#include "font.h"

#ifndef __cplusplus
#include "string.h"
#else
#include "str.h"
#endif

#include "tab_cnv.h"

#include "maths.h"
#include "maths3D.h"

#include "vdp.h"
#include "vdp_bg.h"
#include "vdp_spr.h"
#include "vdp_tile.h"

#include "pal.h"

#include "vram.h"
#include "dma.h"

#include "map.h"

#include "bmp.h"
#include "sprite_eng.h"

#include "sound.h"
#include "xgm.h"
#include "z80_ctrl.h"
#include "ym2612.h"
#include "psg.h"
#include "joy.h"
#include "timer.h"

#include "task.h"

// modules
#if (MODULE_EVERDRIVE != 0)
#include "ext/everdrive.h"
#endif

#if (MODULE_FAT16 != 0)
#include "ext/fat16.h"
#endif

#if (MODULE_MEGAWIFI != 0)
#include "ext/mw/megawifi.h"
#endif

#if (MODULE_FLASHSAVE != 0)
#include "ext/flash-save/flash.h"
#include "ext/flash-save/saveman.h"
#endif

#if (MODULE_CONSOLE != 0)
#include "ext/console.h"
#endif

#if (MODULE_FRACTAL != 0)
#include "ext/fractal/fractal.h"
#include "ext/fractal/visual_dbg.h"
#endif

// preserve compatibility with old resources name
#define logo_lib sgdk_logo
#define font_lib font_default
#define font_pal_lib font_pal_default

#ifdef __cplusplus
}
#endif

#endif // _GENESIS_H_
