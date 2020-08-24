#include "HwData.h"

// TODO: Consider removing this.
ROMDATA void* ROM[0x100000 / sizeof(void*)];

RAMDATA void* WORKRAM[0x100000 / sizeof(void*)];
RAMDATA uint8_t INPUTS[NUMINPUTS] = { 0xFFu, 0xFFu, 0xFFu, 0xFFu };
RAMDATA uint8_t EEPROM[4];

RAMDATA uint8_t SOUNDCTRL[8];
uint8_t SoundControlRead(size_t i) {
	// TODO
	return 0u;
}
void SoundControlWrite(size_t i, uint8_t value) {
	// TODO
}

RAMDATA uint32_t GRAPHICSRAM[0x10000 / sizeof(uint32_t)];
RAMDATA Color PALRAM[NUMPALS * NUMPALCOLORS_4BPP];
RAMDATA uint16_t SCALERAM[0x100];
RAMDATA uint8_t VIDEOCTRL;

RAMDATA uint8_t IRQCTRL[4];
uint8_t InterruptControlRead(size_t i) {
	// TODO
	return 0u;
}
void InterruptControlWrite(size_t i, uint8_t value) {
	// TODO
}

RAMDATA uint8_t VIDEOREGS[0x20];

RAMDATA uint32_t GRAPHICSCHECK[0x20000 / sizeof(uint32_t)];
void GraphicsCheckSetBank(size_t i, uint8_t value) {
	// TODO
}

RAMDATA uint8_t UNK_FFFFFE10;
RAMDATA uint8_t UNK_FFFFFE11;
RAMDATA uint8_t UNK_FFFFFE12;
RAMDATA uint8_t UNK_FFFFFE12;
RAMDATA uint8_t UNK_FFFFFE13;
RAMDATA uint8_t UNK_FFFFFE14;
RAMDATA uint8_t UNK_FFFFFE15;
RAMDATA uint8_t UNK_FFFFFE16;

RAMDATA uint16_t UNK_FFFFFE60;
RAMDATA uint16_t UNK_FFFFFE66;

RAMDATA uint8_t UNK_FFFFFE92;

RAMDATA uint32_t UNK_FFFFFFE0;
RAMDATA uint32_t UNK_FFFFFFE4;
RAMDATA uint32_t UNK_FFFFFFE8;
RAMDATA uint32_t UNK_FFFFFFEC;
RAMDATA uint32_t UNK_FFFFFFF0;
RAMDATA uint32_t UNK_FFFFFFF8;