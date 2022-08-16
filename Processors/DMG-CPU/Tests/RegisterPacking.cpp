#include <Kiwi/DMG-CPU/Processor.hpp>
#include <Kiwi/Utility/Log.hpp>

#include <gtest/gtest.h>

using namespace kiwi;
using namespace kiwi::DMG_CPU;

TEST(RegisterPacking, All) {
    Processor P;

    // CPU Registers

    P.F = 0b10000000;
    ASSERT_TRUE(P.FC);
    ASSERT_FALSE(P.FH);
    ASSERT_FALSE(P.FN);
    ASSERT_FALSE(P.FZ);

    P.F = 0b01000000;
    ASSERT_FALSE(P.FC);
    ASSERT_TRUE(P.FH);
    ASSERT_FALSE(P.FN);
    ASSERT_FALSE(P.FZ);

    P.F = 0b00100000;
    ASSERT_FALSE(P.FC);
    ASSERT_FALSE(P.FH);
    ASSERT_TRUE(P.FN);
    ASSERT_FALSE(P.FZ);

    P.F = 0b00010000;
    ASSERT_FALSE(P.FC);
    ASSERT_FALSE(P.FH);
    ASSERT_FALSE(P.FN);
    ASSERT_TRUE(P.FZ);

    P.A = 0xAA;
    P.F = 0xFF;
    ASSERT_EQ(P.AF, 0xAAFF);
    
    P.B = 0xBB;
    P.C = 0xCC;
    ASSERT_EQ(P.BC, 0xBBCC);
    
    P.D = 0xDD;
    P.E = 0xEE;
    ASSERT_EQ(P.DE, 0xDDEE);
    
    P.H = 0xFF;
    P.L = 0x00;
    ASSERT_EQ(P.HL, 0xFF00);

    // Interrupt Flags (IF & IE)

    P.IF.raw = 0b00010000;
    ASSERT_TRUE(P.IF.Int60);
    ASSERT_FALSE(P.IF.Int58);
    ASSERT_FALSE(P.IF.Int50);
    ASSERT_FALSE(P.IF.Int48);
    ASSERT_FALSE(P.IF.Int40);
    
    P.IF.raw = 0b00001000;
    ASSERT_FALSE(P.IF.Int60);
    ASSERT_TRUE(P.IF.Int58);
    ASSERT_FALSE(P.IF.Int50);
    ASSERT_FALSE(P.IF.Int48);
    ASSERT_FALSE(P.IF.Int40);
    
    P.IF.raw = 0b00000100;
    ASSERT_FALSE(P.IF.Int60);
    ASSERT_FALSE(P.IF.Int58);
    ASSERT_TRUE(P.IF.Int50);
    ASSERT_FALSE(P.IF.Int48);
    ASSERT_FALSE(P.IF.Int40);
    
    P.IF.raw = 0b00000010;
    ASSERT_FALSE(P.IF.Int60);
    ASSERT_FALSE(P.IF.Int58);
    ASSERT_FALSE(P.IF.Int50);
    ASSERT_TRUE(P.IF.Int48);
    ASSERT_FALSE(P.IF.Int40);
    
    P.IF.raw = 0b00000001;
    ASSERT_FALSE(P.IF.Int60);
    ASSERT_FALSE(P.IF.Int58);
    ASSERT_FALSE(P.IF.Int50);
    ASSERT_FALSE(P.IF.Int48);
    ASSERT_TRUE(P.IF.Int40);

    // LCD STAT
    
    P.STAT.raw = 0b01000000;
    ASSERT_TRUE(P.STAT.IntCoincidence);
    ASSERT_FALSE(P.STAT.IntSearchSprite);
    ASSERT_FALSE(P.STAT.IntVBlank);
    ASSERT_FALSE(P.STAT.IntHBlank);
    ASSERT_FALSE(P.STAT.Coincidence);
    ASSERT_EQ(P.STAT.Mode, STAT_MODE_HBLANK);
    
    P.STAT.raw = 0b00100000;
    ASSERT_FALSE(P.STAT.IntCoincidence);
    ASSERT_TRUE(P.STAT.IntSearchSprite);
    ASSERT_FALSE(P.STAT.IntVBlank);
    ASSERT_FALSE(P.STAT.IntHBlank);
    ASSERT_FALSE(P.STAT.Coincidence);
    ASSERT_EQ(P.STAT.Mode, STAT_MODE_HBLANK);
    
    P.STAT.raw = 0b00010000;
    ASSERT_FALSE(P.STAT.IntCoincidence);
    ASSERT_FALSE(P.STAT.IntSearchSprite);
    ASSERT_TRUE(P.STAT.IntVBlank);
    ASSERT_FALSE(P.STAT.IntHBlank);
    ASSERT_FALSE(P.STAT.Coincidence);
    ASSERT_EQ(P.STAT.Mode, STAT_MODE_HBLANK);
    
    P.STAT.raw = 0b00001000;
    ASSERT_FALSE(P.STAT.IntCoincidence);
    ASSERT_FALSE(P.STAT.IntSearchSprite);
    ASSERT_FALSE(P.STAT.IntVBlank);
    ASSERT_TRUE(P.STAT.IntHBlank);
    ASSERT_FALSE(P.STAT.Coincidence);
    ASSERT_EQ(P.STAT.Mode, STAT_MODE_HBLANK);
    
    P.STAT.raw = 0b00000100;
    ASSERT_FALSE(P.STAT.IntCoincidence);
    ASSERT_FALSE(P.STAT.IntSearchSprite);
    ASSERT_FALSE(P.STAT.IntVBlank);
    ASSERT_FALSE(P.STAT.IntHBlank);
    ASSERT_TRUE(P.STAT.Coincidence);
    ASSERT_EQ(P.STAT.Mode, STAT_MODE_HBLANK);
    
    P.STAT.raw = 0b00000011;
    ASSERT_FALSE(P.STAT.IntCoincidence);
    ASSERT_FALSE(P.STAT.IntSearchSprite);
    ASSERT_FALSE(P.STAT.IntVBlank);
    ASSERT_FALSE(P.STAT.IntHBlank);
    ASSERT_FALSE(P.STAT.Coincidence);
    ASSERT_EQ(P.STAT.Mode, STAT_MODE_DATA_TRANSFER);

}

int main(int argc, char * argv[]) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}