// TODO: Move the VIDEOREGS defines to more specific headers, like HwSprite.h.
#pragma once

#include "PlatformTypes.h"
#include "HwSprite.h"
#include "HwInput.h"
#include "VideoDefs.h"
#include "Macros.h"
#include <stdint.h>

// NOTE: Don't cast and access these with other types. TAP's SH-2 code accesses
// these using specifically sized types, and that might be required on
// PsikyoSH. The only exceptions to that rule would be accessing ROM and
// WORKRAM, which can be accessed mostly freely.

extern ROMDATA void* ROM[0x100000 / sizeof(void*)];

extern RAMDATA void* WORKRAM[0x100000 / sizeof(void*)];

extern RAMDATA uint8_t INPUTS[NUMINPUTS];

extern RAMDATA uint8_t EEPROM[4];

extern RAMDATA uint8_t SOUNDCTRL[8];

extern RAMDATA void* GRAPHICSRAM[0x10000 / sizeof(void*)];
#define SPRITERAM ((RAMDATA uint16_t*)&GRAPHICSRAM[0x00000 / sizeof(void*)])
#define BGRAM (&GRAPHICSRAM[0x004000 / sizeof(void*)])
#define BACKDROPRAM ((RAMDATA Color*)(&BGRAM[0x100 / sizeof(void*)]))
#define NUMBACKDROPLINES 0x100u

#define NUMPALS 0x100u
Color PALRAM[NUMPALS * NUMPALCOLORS_4BPP];

extern RAMDATA uint16_t SCALERAM[0x100];

extern RAMDATA uint8_t VIDEOCTRL;

extern RAMDATA uint8_t IRQCTRL[4];

extern RAMDATA uint8_t VIDEOREGS[0x20];
#define NUMALPHAVALUES 8u
#define AlphaValues (&VIDEOREGS[0x0])
#define SpritePriority (&VIDEOREGS[0x08])
#define _0x2405FFEA (VIDEOREGS[0x0A])
#define _0x2405FFEB (VIDEOREGS[0x0B])
#define _0x2405FFEC (VIDEOREGS[0x0C])
#define _0x2405FFED (VIDEOREGS[0x0D])
#define VideoSettings (&VIDEOREGS[0x0E])
#define _0x2405FFF0 (VIDEOREGS[0x10])
#define _0x2405FFF1 (VIDEOREGS[0x11])
#define _0x2405FFF2 (VIDEOREGS[0x12])
#define _0x2405FFF3 (VIDEOREGS[0x13])
#define _0x2405FFF4 (VIDEOREGS[0x14])
#define _0x2405FFF5 (VIDEOREGS[0x15])
#define _0x2405FFF6 (VIDEOREGS[0x16])
#define _0x2405FFF7 (VIDEOREGS[0x17])
#define BgMapBanks (&VIDEOREGS[0x18])
#define ScanlinesBank (&VIDEOREGS[0x1C])
#define BgMapSettings (&VIDEOREGS[0x1E])

extern RAMDATA uint8_t _0xFFFFFE10;
extern RAMDATA uint8_t _0xFFFFFE11;
extern RAMDATA uint8_t _0xFFFFFE12;
extern RAMDATA uint8_t _0xFFFFFE12;
extern RAMDATA uint8_t _0xFFFFFE13;
extern RAMDATA uint8_t _0xFFFFFE14;
extern RAMDATA uint8_t _0xFFFFFE15;
extern RAMDATA uint8_t _0xFFFFFE16;

extern RAMDATA uint16_t _0xFFFFFE60;
extern RAMDATA uint16_t _0xFFFFFE66;

extern RAMDATA uint8_t _0xFFFFFE92;

extern RAMDATA uint32_t _0xFFFFFFE0;
extern RAMDATA uint32_t _0xFFFFFFE4;
extern RAMDATA uint32_t _0xFFFFFFE8;
extern RAMDATA uint32_t _0xFFFFFFEC;
extern RAMDATA uint32_t _0xFFFFFFF0;
extern RAMDATA uint32_t _0xFFFFFFF8;
