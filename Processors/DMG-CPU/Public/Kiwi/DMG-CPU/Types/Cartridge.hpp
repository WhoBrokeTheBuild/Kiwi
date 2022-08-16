#ifndef KIWI_DMG_CPU_TYPES_Cartridge_HPP
#define KIWI_DMG_CPU_TYPES_Cartridge_HPP

#include <cstdint>

namespace kiwi {
namespace DMG_CPU {

enum class MBCType
{
    None,
    MBC1,
    MBC2,
    MBC3,
    MBC5,
    MBC6,
    MBC7,
    MMM01,

}; // enum class MBCType

union CartridgeHeader
{
    struct {
        // $0100-$0103 Entry Point Code, usually a JP instruction
        uint8_t Code[4];

        // $0104-$0133 Nintendo Logo, used for verification
        uint8_t Magic[48];

        // $0134-$0143 ROM Title String, zero padded
        char Title[15];

        // $0143 Determines if CGB functionality is requested and potentially required
        uint8_t CGBFlag;

        // $0144-$0145 Code for determining the game's publisher, used after the SGB was released
        char NewLicenseeCode[2];

        // $0146 Determines if SGB functionality is requested
        uint8_t SGBFlag;

        // $0147 Specifies the Memory Bank Controller used, if any, and if any external hardware is in the cartridge
        uint8_t CartridgeType;

        // $0148 ROM Size
        uint8_t ROMSizeCode;

        // $0149 SRAM Size
        uint8_t SRAMSizeCode;

        // $014A Determines if the game is to be sold in Japan or elsewhere
        uint8_t RegionCode;

        // $014B Code for determining the game's publisher, used before the SGB was relased
        uint8_t LicenseeCode;

        // $014C Version number of the game, usually 0
        uint8_t Version;

        // $014D Checksum of bytes $0134-$014C, required
        uint8_t HeaderChecksum;

        // $014E-$014F Checksum of entire ROM, not required
        uint16_t GlobalChecksum;
    };

    uint8_t raw[80];

}; // union CartridgeHeader

static_assert(
    sizeof(CartridgeHeader) == sizeof(CartridgeHeader::raw),
    "sizeof(CartridgeHeader) != sizeof(CartridgeHeader::raw)"
);

} // namespace DMG_CPU
} // namespace kiwi

#endif // KIWI_DMG_CPU_TYPES_Cartridge_HPP