// TODO: Consider changing the Item struct to a forward declaration, and moving
// the struct into Item.c.
#pragma once

#include "Lib/Macros.h"
#include <stdint.h>
#include <stdbool.h>
struct Player;

typedef enum ItemType {
	ITEMTYPE_NULL,
	ITEMTYPE_DEATHBLOCK,
	ITEMTYPE_NEGAFIELD,
	ITEMTYPE_COLORBLOCK,
	ITEMTYPE_TOPDELFIELD,
	ITEMTYPE_BOTTOMDELFIELD,
	ITEMTYPE_RIGHTMOVFIELD,
	ITEMTYPE_LEFTMOVFIELD,
	ITEMTYPE_DELEVEN,
	ITEMTYPE_FLIPFIELD,
	ITEMTYPE_SHOTGUN,
	ITEMTYPE_HARDBLOCK,
	ITEMTYPE_LASER,
	ITEMTYPE_ROLLROLL,
	ITEMTYPE_TRANSFORM,
	ITEMTYPE_FREEFALL,
	ITEMTYPE_XRAY,
	ITEMTYPE_EXCHGFIELD,
	ITEMTYPE_MIRRORBLOCK,
	ITEMTYPE_DARKBLOCK,
	NUMITEMTYPES = ITEMTYPE_DARKBLOCK,
	ITEMTYPE_GAMEOVER = 0xFF // Not actually an item.
} ItemType;
#define TOITEMNUM(itemType) ((uint8_t)((itemType) - 1))
#define TOITEMTYPE(itemNum) ((ItemType)((itemNum) + 1))

typedef enum ItemMiscFlag {
	ITEMMISC_NONE,
	ITEMMISC_DEATHBLOCK = 1 << 0,
	ITEMMISC_ROTATE = 1 << 1,
	ITEMMISC_BIT10 = 1 << 10
} ItemMiscFlag;

typedef enum ItemBagFlag {
	ITEMBAG_NONE = 0x00000,
	ITEMBAG_DEATHBLOCK = 1 << TOITEMNUM(ITEMTYPE_DEATHBLOCK),
	ITEMBAG_NEGAFIELD = 1 << TOITEMNUM(ITEMTYPE_NEGAFIELD),
	ITEMBAG_COLORBLOCK = 1 << TOITEMNUM(ITEMTYPE_COLORBLOCK),
	ITEMBAG_TOPDELFIELD = 1 << TOITEMNUM(ITEMTYPE_TOPDELFIELD),
	ITEMBAG_BOTTOMDELFIELD = 1 << TOITEMNUM(ITEMTYPE_BOTTOMDELFIELD),
	ITEMBAG_RIGHTMOVFIELD = 1 << TOITEMNUM(ITEMTYPE_RIGHTMOVFIELD),
	ITEMBAG_LEFTMOVFIELD = 1 << TOITEMNUM(ITEMTYPE_LEFTMOVFIELD),
	ITEMBAG_DELEVEN = 1 << TOITEMNUM(ITEMTYPE_DELEVEN),
	ITEMBAG_FLIPFIELD = 1 << TOITEMNUM(ITEMTYPE_FLIPFIELD),
	ITEMBAG_SHOTGUN = 1 << TOITEMNUM(ITEMTYPE_SHOTGUN),
	ITEMBAG_HARDBLOCK = 1 << TOITEMNUM(ITEMTYPE_HARDBLOCK),
	ITEMBAG_LASER = 1 << TOITEMNUM(ITEMTYPE_LASER),
	ITEMBAG_ROLLROLL = 1 << TOITEMNUM(ITEMTYPE_ROLLROLL),
	ITEMBAG_TRANSFORM = 1 << TOITEMNUM(ITEMTYPE_TRANSFORM),
	ITEMBAG_FREEFALL = 1 << TOITEMNUM(ITEMTYPE_FREEFALL),
	ITEMBAG_XRAY = 1 << TOITEMNUM(ITEMTYPE_XRAY),
	ITEMBAG_EXCHGFIELD = 1 << TOITEMNUM(ITEMTYPE_EXCHGFIELD),
	ITEMBAG_MIRRORBLOCK = 1 << TOITEMNUM(ITEMTYPE_MIRRORBLOCK),
	ITEMBAG_DARKBLOCK = 1 << TOITEMNUM(ITEMTYPE_DARKBLOCK),
	ITEMBAG_FULL = 0x7FFFF
} ItemBagFlag;

#define MAXITEMBARBLOCKS 20u

typedef enum ItemStatus {
	ITEMSTATUS_FREE,
	ITEMSTATUS_ACTIVE,
	ITEMSTATUS_INACTIVE
} ItemStatus;

typedef enum ItemState {
	ITEMSTATE_DEFAULT = 99
} ItemState;

typedef struct Item Item;
struct Item {
	ItemStatus status;
	ItemType type;

	int16_t values[3];
	uint8_t states[4];
	// NOTE: The data store was originally 0x800 bytes in the SH-2 code, and
	// that worked because data is pretty tightly packed in that version, but I
	// found MatrixBlock is 16 bytes with x64 builds created by Visual Studio
	// 2019, so the data store needs expansion to work there. The need for
	// expanded storage came up while I was working on the mirror block item.
	// -Brandon McGriff
	DATA(data, 0x1000);

	struct Player* activatingPlayer;

	Item* previous;
	Item* next;
};

typedef enum ItemCategory {
	ITEMCAT_NONE,
	ITEMCAT_GOOD,
	ITEMCAT_BAD,
	ITEMCAT_NEUTRAL
} ItemCategory;

void RemoveItems(struct Player* player);
bool CheckDeactivateItem(Item* item);
bool FreeFall(struct Player* player);
bool UNK_6018584(struct Player* player); // Unused.
ItemCategory NoDisableGarbage(Item* item);

bool ItemGood(struct Player* player);
bool ItemBad(struct Player* player);
ItemCategory GetItemCategory(ItemType itemType);
bool ItemConfusing(struct Player* player);

void UNK_601886C();
// NOTE: Unused. Not suggested to be used, since it doesn't work the same as
// UpdateItems(), and the rest of the code depends on how UpdateItems() works.
// Only included for the sake of completeness and preservation.
//void UpdateVersusItems(struct Player* player);

// Init the items.
void InitItems();
Item* AllocItem();
void DeactivateItem(Item* item);
void UpdateItems();
