#ifndef KIWI_DMG_CPU_Constants_HPP
#define KIWI_DMG_CPU_Constants_HPP

namespace kiwi {
namespace DMG_CPU {

// Size of Bootstrap ROM
inline const unsigned BOOTSTRAP_ROM_SIZE = 0x100;

// Maximum number of Cartridge ROM Banks
inline const unsigned MAX_CARTRIDGE_ROM_BANK_COUNT = 512;

// Size of each Cartridge ROM Bank
inline const unsigned CARTRIDGE_ROM_BANK_SIZE = 0x4000;

// Number of Static RAM Banks
inline const unsigned STATIC_RAM_BANK_COUNT = 8;

// Size of each Static RAM Bank
inline const unsigned STATIC_RAM_BANK_SIZE = 0x2000;

// Number of Work RAM Banks
inline const unsigned WORK_RAM_BANK_COUNT = 8;

// Size of each Work RAM Bank
inline const unsigned WORK_RAM_BANK_SIZE = 0x1000;

// Number of Video RAM Banks
inline const unsigned VIDEO_RAM_BANK_COUNT = 2;

// Size of each Video RAM Bank
inline const unsigned VIDEO_RAM_BANK_SIZE = 0x2000;

// Size of Object Attribute Memory
inline const unsigned OAM_SIZE = 0xA0;

// Size of the Wave RAM
inline const unsigned WAVE_RAM_SIZE = 0x10;

// Size of the High RAM
inline const unsigned HIGH_RAM_SIZE = 0x7F;

// Location of the Cartridge Header as an offset into CartROM[0]
inline const unsigned CARTRIDGE_HEADER_OFFSET = 0x0100;

// Screen Width in Pixels
inline const unsigned SCREEN_WIDTH = 160;

// Screen Height in Pixels
inline const unsigned SCREEN_HEIGHT = 144;

// Screen Buffer Components (RGB)
inline const unsigned SCREEN_BUFFER_COMPONENTS = 4;

// Size of Screen Buffer in Bytes
inline const unsigned SCREEN_BUFFER_SIZE = (SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BUFFER_COMPONENTS);

// Clock Speed DMG CPU in Hertz
inline const unsigned CLOCK_SPEED_DMG = 4194304;

// Clock Speed SGB CPU in Hertz
inline const unsigned CLOCK_SPEED_SGB = 4295454;

// Clock Speed CGB CPU in Double-Speed Mode in Hertz
inline const unsigned CLOCK_SPEED_CGB = 8388608;

// Horizontal Blanking Period
inline const unsigned STAT_MODE_HBLANK = 0b00;

// Vertical Blanking Period
inline const unsigned STAT_MODE_VBLANK = 0b01;

// Scanning OAM for sprites to be drawn on this line
inline const unsigned STAT_MODE_SEARCH_SPRITE = 0b10;

// Reading OAM and VRAM to generate pixels
inline const unsigned STAT_MODE_DATA_TRANSFER = 0b11;

// Regions for accessing tile maps
inline const unsigned TILE_MAP_ADDRESS[] = {
    0x9800, // $9800 - $9BFF
    0x9C00, // $9C00 - $9FFF
};

// Regions for accessing tile data
inline const unsigned TILE_DATA_ADDRESS[] = {
    0x9000, // $8800 - $97FF
    0x8000, // $8000 - $8FFF
};

inline const unsigned TILE_WIDTH = 8;

inline const unsigned TILE_HEIGHT = 8;

inline const unsigned TILES_PER_ROW = 2;

inline const unsigned TILE_DATA_SIZE = 6;


} // namespace DMG_CPU
} // namespace kiwi

#endif // KIWI_DMG_CPU_Constants_HPP