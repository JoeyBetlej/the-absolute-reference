#include "Platform/Util/Render.h"
#include "Video/VideoDefs.h"
#include "Video/Object.h"
#include "Video/HwSprite.h"
#include "HwData.h"
#include "Lib/Macros.h"
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define PIXELALPHA (UINT32_C(~0))

static const uint32_t AlphaTable[0x100] = {
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFBu, 0xF7u, 0xF3u, 0xEFu, 0xEBu, 0xE7u, 0xE3u, 0xDFu, 0xDBu, 0xD7u, 0xD3u, 0xCFu, 0xCBu, 0xC7u, 0xC3u,
	0xBEu, 0xBAu, 0xB6u, 0xB2u, 0xAEu, 0xAAu, 0xA6u, 0xA2u, 0x9Eu, 0x9Au, 0x96u, 0x92u, 0x8Eu, 0x8Au, 0x86u, 0x82u,
	0x7Du, 0x79u, 0x75u, 0x71u, 0x6Du, 0x69u, 0x65u, 0x61u, 0x5Du, 0x59u, 0x55u, 0x51u, 0x4Du, 0x49u, 0x45u, 0x41u,
	0x3Cu, 0x38u, 0x34u, 0x30u, 0x2Cu, 0x28u, 0x24u, 0x20u, 0x1Cu, 0x18u, 0x14u, 0x10u, 0x0Cu, 0x08u, 0x04u, 0x00u
};

static void RenderClearRasters(Color* const framebuffer) {
	RAMDATA Color* const clearRasters = ClearRasters;

	for (int16_t y = 0; y < VIDEO_HEIGHT; y++) {
		for (int16_t x = 0; x < VIDEO_WIDTH; x++) {
			framebuffer[y * VIDEO_WIDTH + x] = clearRasters[y];
		}
	}
}

#define GETSPRITEPRIORITY(priority) ((SpritePriority[(priority) / 2u] >> (4u * (((priority) + 1u) % 2u))) & 0xFu)

static void RenderSprites(Color* const framebuffer, const uint8_t* const tileData, const uint8_t priority) {
	if (!(SpriteNames[0] & SPRITENAME_TERMINATE) && !(SpriteNames[1] & SPRITENAME_TERMINATE)) {
		for (size_t i = SPRITE_FIRST; i < MAXSPRITES; i++) {
			const SpriteData* const sprite = &Sprites[SpriteNames[i] & ~SPRITENAME_TERMINATE];
			const int16_t y = OBJECT_GETY(sprite);
			const int16_t x = OBJECT_GETX(sprite);
			const bool flipY = OBJECT_GETFLIPY(sprite);
			//const bool unknown0 = OBJECT_GETUNKNOWN0(sprite);
			//const uint8_t sprPri = OBJECT_GETSPRPRI(sprite);
			const uint32_t h = OBJECT_GETH(sprite) + 1u;
			const int32_t scaleY = SCALERAM[OBJECT_GETSCALEY(sprite)];
			const bool flipX = OBJECT_GETFLIPX(sprite);
			//const bool unknown1 = OBJECT_GETUNKNOWN1(sprite);
			const uint8_t bgPri = OBJECT_GETBGPRI(sprite);
			const uint32_t w = OBJECT_GETW(sprite) + 1u;
			const uint32_t scaleX = SCALERAM[OBJECT_GETSCALEX(sprite)];
			const uint32_t palNum = OBJECT_GETPALNUM(sprite);
			const Bpp bpp = OBJECT_GETBPP(sprite);
			uint32_t alphaTemp = Alpha[OBJECT_GETALPHA(sprite)];

			if (GETSPRITEPRIORITY(bgPri) != priority) {
				if (SpriteNames[i] & SPRITENAME_TERMINATE) {
					break;
				}
				else {
					continue;
				}
			}

			if (alphaTemp & 0x80u) {
				alphaTemp = PIXELALPHA;
			}
			else {
				alphaTemp = 0x3Fu - (alphaTemp & 0x3Fu);
				alphaTemp = (alphaTemp << 2) | (alphaTemp >> 4);
			}
			const uint32_t alpha = alphaTemp;
			const uint32_t tile = OBJECT_GETTILE(sprite);

			const int16_t renderW = (((w << 24) / scaleX) + 512) >> 10;
			const int16_t renderH = (((h << 24) / scaleY) + 512) >> 10;
			for (int16_t offsetY = 0; offsetY < renderH; offsetY++) {
				for (int16_t offsetX = 0; offsetX < renderW; offsetX++) {
					// Skip offscreen pixels.
					if (
						x + offsetX < 0 ||
						x + offsetX >= VIDEO_WIDTH ||
						y + offsetY < 0 ||
						y + offsetY >= VIDEO_HEIGHT
					) {
						continue;
					}

					const int32_t sourceX = ((flipX ? renderW - 1 - offsetX : offsetX) * scaleX) / 1024;
					const int32_t sourceY = ((flipY ? renderH - 1 - offsetY : offsetY) * scaleY) / 1024;
					const int32_t tileX = sourceX / 16;
					const int32_t tileY = sourceY / 16;
					const int32_t tileOffsetX = sourceX % 16;
					const int32_t tileOffsetY = sourceY % 16;
					uint8_t palOffset = (
						tileData[
							((tile + w * tileY) * 0x80u * (bpp + 1)) -
							0xC00000u +
							(tileOffsetX / (2 - bpp)) +
							(tileX * 0x80u * (bpp + 1u)) +
							(tileOffsetY * 8u * (bpp + 1u))
						] >> ((4 - ((tileOffsetX & 1) * 4)) * (1u - bpp))
					) & ((0xF0u * bpp) | 0x0Fu);

					if (palOffset == 0u) {
						continue;
					}
					else {
						const Color* const color = &PALRAM[palNum * NUMPALCOLORS_4BPP + palOffset];
						uint32_t blendAlpha = ((alpha == PIXELALPHA) ? AlphaTable[palOffset] : alpha) & 0xFFu;
						Color* const pixel = &framebuffer[(y + offsetY) * VIDEO_WIDTH + x + offsetX];
						*pixel = COLOR(
							((uint32_t)COLOR_GETR(*color) * blendAlpha + (uint32_t)COLOR_GETR(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
							((uint32_t)COLOR_GETG(*color) * blendAlpha + (uint32_t)COLOR_GETG(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
							((uint32_t)COLOR_GETB(*color) * blendAlpha + (uint32_t)COLOR_GETB(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
							0u
						);
					}
				}
			}
			if (SpriteNames[i] & SPRITENAME_TERMINATE) {
                break;
			}
		}
	}
}

// TODO: Implement remaining background features. Currently, this just renders the background in the simplest possible way that works for TAP.
static void RenderBg(Color* const framebuffer, const uint8_t* const tileData, const uint8_t priority) {
	for (size_t layer = 0u; layer < 4u; layer++) {
		const size_t bgBank = BgMapBank[layer] & 0x7Fu;
		assert(0x7F0u / sizeof(uint32_t) + (bgBank * 0x800u) / sizeof(uint32_t) + (layer * 4u) / sizeof(uint32_t) < lengthof(GRAPHICSRAM));
		const size_t bgPri    = (GRAPHICSRAM[0x7F0u / sizeof(uint32_t) + (bgBank * 0x800u) / sizeof(uint32_t) + (layer * 4u) / sizeof(uint32_t)] >> 24) & 0xFFu;
		const size_t tileBank = (GRAPHICSRAM[0x7F0u / sizeof(uint32_t) + (bgBank * 0x800u) / sizeof(uint32_t) + (layer * 4u) / sizeof(uint32_t)] >>  0) & 0xFFu;
		if (!(BgMapSetting[layer / 2] & (0x80u >> (layer % 2))) || bgPri != priority || tileBank < 0x0Au || tileBank > 0x20u) {
			continue;
		}

		const size_t bpp = !!(BgMapSetting[layer / 2] & (0x40u >> (layer % 2)));
		const size_t h = (BgMapSetting[layer / 2] & (0x10u >> (layer % 2))) ? 0x200u : 0x100u;

		const bool pixelAlpha = (GRAPHICSRAM[0x7F0u / sizeof(uint32_t) + (bgBank * 0x800u) / sizeof(uint32_t) + (layer * 4u) / sizeof(uint32_t)] >> 15) & 0x01u;
		uint32_t    alphaTemp = (GRAPHICSRAM[0x7F0u / sizeof(uint32_t) + (bgBank * 0x800u) / sizeof(uint32_t) + (layer * 4u) / sizeof(uint32_t)] >>  8) & 0x3Fu;
		if (pixelAlpha) {
			alphaTemp = PIXELALPHA;
		}
		else {
			alphaTemp = 0x3Fu - (alphaTemp & 0x3Fu);
			alphaTemp = (alphaTemp << 2) | (alphaTemp >> 4);
		}
		const uint32_t alpha = alphaTemp;

		assert(VIDEO_HEIGHT % 16u == 0u && VIDEO_WIDTH % 16u == 0u);
		for (size_t tileY = 0u; tileY < VIDEO_HEIGHT / 16u; tileY++) {
			for (size_t tileX = 0u; tileX < VIDEO_WIDTH / 16u; tileX++) {
				const uint32_t bgTile = GRAPHICSRAM[(tileBank * 0x800u) / sizeof(uint32_t) + (((tileY & 0x1Fu) << 5) | (tileX & 0x1Fu))];
				const size_t tile = bgTile & 0x7FFFFu;
				assert(tile >= 0xC000u * (bpp + 1u));
				const size_t palNum = bgTile >> 24;

				if (tile == 0u) {
					continue;
				}

				for (size_t tileOffsetY = 0u; tileOffsetY < 16u; tileOffsetY++) {
					for (size_t tileOffsetX = 0u; tileOffsetX < 16u; tileOffsetX++) {
						const size_t palOffset = tileData[(tile * 0x100u) - 0xC00000u + tileOffsetY * 16u + tileOffsetX];
						if (palOffset == 0u) {
							continue;
						}

						const Color color = PALRAM[palNum * NUMPALCOLORS_4BPP + palOffset];
						uint32_t blendAlpha = ((alpha == PIXELALPHA) ? AlphaTable[palOffset] : alpha) & 0xFFu;
						Color* const pixel = &framebuffer[(tileY * 16u + tileOffsetY) * VIDEO_WIDTH + tileX * 16u + tileOffsetX];
						*pixel = COLOR(
							((uint32_t)COLOR_GETR(color) * blendAlpha + (uint32_t)COLOR_GETR(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
							((uint32_t)COLOR_GETG(color) * blendAlpha + (uint32_t)COLOR_GETG(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
							((uint32_t)COLOR_GETB(color) * blendAlpha + (uint32_t)COLOR_GETB(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
							0u
						);
					}
				}
			}
		}
	}
}

static void RenderOverlayRasters(Color* const framebuffer, const uint8_t priority) {
	if ((UNK_2405FFEB & 0xFu) != priority) {
		return;
	}

	RAMDATA Color* rasters = OverlayRasters;
	for (int16_t y = 0; y < VIDEO_HEIGHT; y++) {
		if (COLOR_GETA(rasters[y]) & 0x80u) {
			for (int16_t x = 0; x < VIDEO_WIDTH; x++) {
				framebuffer[y * VIDEO_WIDTH + x] = rasters[y];
			}
		}
		else if (COLOR_GETA(rasters[y]) & 0x7Fu) {
			const uint32_t blendAlpha = (COLOR_GETA(rasters[y]) & 0x7Fu) * 2u;
			for (int16_t x = 0; x < VIDEO_WIDTH; x++) {
				Color* const pixel = &framebuffer[y * VIDEO_WIDTH + x];
				*pixel = COLOR(
					((uint32_t)COLOR_GETR(rasters[y]) * blendAlpha + (uint32_t)COLOR_GETR(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
					((uint32_t)COLOR_GETG(rasters[y]) * blendAlpha + (uint32_t)COLOR_GETG(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
					((uint32_t)COLOR_GETB(rasters[y]) * blendAlpha + (uint32_t)COLOR_GETB(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
					0u
				);
			}
		}
	}
}

void Render(Color* const framebuffer, const uint8_t* const tileData) {
	RenderClearRasters(framebuffer);
	for (size_t priority = 0u; priority < 8u; priority++) {
		RenderSprites(framebuffer, tileData, priority);
		RenderBg(framebuffer, tileData, priority);
		RenderOverlayRasters(framebuffer, priority);
	}
}
