#include <Kiwi/DMG-CPU/Processor.hpp>
#include <Kiwi/Utility/Log.hpp>

namespace kiwi {
namespace DMG_CPU {

void Processor::tickPPU()
{
    const uint8_t LCD_COLORS[][3] = {
        { 0xFF, 0xFF, 0xFF },
        { 0xCC, 0xCC, 0xCC },
        { 0x77, 0x77, 0x77 },
        { 0x00, 0x00, 0x00 },
    };

    switch (STAT.Mode) {
    
    case STAT_MODE_SEARCH_SPRITE:
        if (_lineTicks == 40) {
            STAT.Mode = STAT_MODE_DATA_TRANSFER;
            // Log(KIWI_ANCHOR, "Mode -> Data Transfer");
        }
        break;

    case STAT_MODE_DATA_TRANSFER:
        if (_lineTicks == 200) {
            STAT.Mode = STAT_MODE_HBLANK;
            // Log(KIWI_ANCHOR, "Mode -> HBlank");
        }
        break;

    case STAT_MODE_HBLANK:
        if (_lineTicks == 456) {
            _lineTicks = 0;

            bool usingWindow = (LCDC.WindowEnable && WY <= LY);
            uint8_t mapSelect = (usingWindow ? LCDC.WindowTileMapSelect : LCDC.BGTileMapSelect);
            uint16_t mapBaseAddress = TILE_MAP_ADDRESS[mapSelect];
            uint16_t dataBaseAddress = TILE_DATA_ADDRESS[LCDC.TileDataSelect];

            unsigned y = (usingWindow ? LY - WY : SCY + LY);
            // Log(KIWI_ANCHOR, "{}", y);

            unsigned tileRow = y / TILE_HEIGHT;

            for (unsigned i = 0; i < SCREEN_WIDTH; ++i) {
                unsigned x = i + SCX;
                if (usingWindow && i >= (WX - 7)) {
                    x = i - WX - 7;
                }

                unsigned tileCol = x / TILE_WIDTH;

                int tileIndex = readByte(mapBaseAddress + (tileRow * TILES_PER_ROW) + tileCol);
                if (LCDC.TileDataSelect == 0) {
                    tileIndex = (int8_t)tileIndex; // Convert to [-128, 127]
                }

                uint16_t dataOffset = dataBaseAddress + (tileIndex * TILE_DATA_SIZE);

                unsigned line = (y % TILE_HEIGHT) * 2; // ?
                unsigned data1 = readByte(dataOffset + line);
                unsigned data2 = readByte(dataOffset + line + 1);

                unsigned bit = 0x80 >> (x % TILE_WIDTH);
                bool high = (data2 & bit);
                bool low = (data1 & bit);
                uint8_t colorIndex = (high ? 0b10 : 0b00) | (low ? 0b01 : 0b00);

                switch (colorIndex) {
                case 0:
                    colorIndex = BGP.Color0;
                    break;
                case 1:
                    colorIndex = BGP.Color1;
                    break;
                case 2:
                    colorIndex = BGP.Color2;
                    break;
                case 3:
                    colorIndex = BGP.Color3;
                    break;
                }

                const uint8_t * color = LCD_COLORS[colorIndex];

                unsigned offset = ((LY * SCREEN_WIDTH) + i) * SCREEN_BUFFER_COMPONENTS;

                LCD[offset + 0] = color[0];
                LCD[offset + 1] = color[1];
                LCD[offset + 2] = color[2];
                LCD[offset + 3] = 0xFF;
            }
            
            ++LY;
            if (LY == 144) {
                STAT.Mode = STAT_MODE_VBLANK;
                // Log(KIWI_ANCHOR, "Mode -> VBlank");
            }
            else {
                STAT.Mode = STAT_MODE_SEARCH_SPRITE;
                // Log(KIWI_ANCHOR, "Mode -> Search Sprite");
            }
        }
        break;
        
    case STAT_MODE_VBLANK:
        if (_lineTicks == 456) {
            _lineTicks = 0;
            ++LY;

            if (LY == 153) {
                LY = 0;
                STAT.Mode = STAT_MODE_SEARCH_SPRITE;
                // Log(KIWI_ANCHOR, "Mode -> Search Sprite");


                
                // uint16_t dataAddr = TILE_DATA_ADDRESS[LCDC.TileDataSelect];

                // const int SCREEN_STRIDE = SCREEN_WIDTH * SCREEN_BUFFER_COMPONENTS;

                // const int TILE_DRAW_PER_ROW = 16;
                // const int TILE_COUNT = 256;

                // for (int i = 0; i < TILE_COUNT; ++i) {

                //     int x = 1 + ((i % TILE_DRAW_PER_ROW) * (TILE_WIDTH + 1));
                //     int y = 1 + ((i / TILE_DRAW_PER_ROW) * (TILE_HEIGHT + 1));

                //     for (int tileRow = 0; tileRow < TILE_HEIGHT; ++tileRow) {

                //         uint16_t dataOffset = dataAddr;
                //         if (dataAddr == 0x8800) {
                //             dataOffset += ((i - 128) * TILE_DATA_SIZE) + (tileRow * 2);
                //         }
                //         else {
                //             dataOffset += (i * TILE_DATA_SIZE) + (tileRow * 2);
                //         }

                //         uint8_t data1 = readByte(dataOffset);
                //         uint8_t data2 = readByte(dataOffset + 1);

                //         for (int tileCol = 0; tileCol < TILE_WIDTH; ++tileCol) {
                            
                //             unsigned bit = 0x80 >> (x % TILE_WIDTH);
                //             bool high = (data2 & bit);
                //             bool low = (data1 & bit);
                //             uint8_t colorIndex = (high ? 0b10 : 0b00) | (low ? 0b01 : 0b00);

                //             switch (colorIndex) {
                //             case 0:
                //                 colorIndex = BGP.Color0;
                //                 break;
                //             case 1:
                //                 colorIndex = BGP.Color1;
                //                 break;
                //             case 2:
                //                 colorIndex = BGP.Color2;
                //                 break;
                //             case 3:
                //                 colorIndex = BGP.Color3;
                //                 break;
                //             }

                //             const uint8_t * color = LCD_COLORS[colorIndex];

                //             unsigned off = ((y + tileRow) * SCREEN_STRIDE) 
                //                 + ((x + tileCol) * SCREEN_BUFFER_COMPONENTS);

                //             LCD[off + 0] = color[0];
                //             LCD[off + 1] = color[0];
                //             LCD[off + 2] = color[0];
                //         }
                //     }
                // }



            }
        }
        break;
    }

    ++_lineTicks;
}

} // namespace DMG_CPU
} // namespace kiwi