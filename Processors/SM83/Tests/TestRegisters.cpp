#include <gtest/gtest.h>
#include <cstdio>

#include <Kiwi/SM83/SM83.hpp>

using namespace kiwi::SM83;

Processor P;

TEST(BitPacking, Flags) {
    P.F = 0x00;
    EXPECT_EQ(0, P.FZ);
    EXPECT_EQ(0, P.FN);
    EXPECT_EQ(0, P.FH);
    EXPECT_EQ(0, P.FC);

    P.F = 0xFF;
    EXPECT_EQ(1, P.FZ);
    EXPECT_EQ(1, P.FN);
    EXPECT_EQ(1, P.FH);
    EXPECT_EQ(1, P.FC);

    P.F = 0b01010000;
    EXPECT_EQ(1, P.FZ);
    EXPECT_EQ(0, P.FN);
    EXPECT_EQ(1, P.FH);
    EXPECT_EQ(0, P.FC);

    P.F = 0b10101111;
    EXPECT_EQ(0, P.FZ);
    EXPECT_EQ(1, P.FN);
    EXPECT_EQ(0, P.FH);
    EXPECT_EQ(1, P.FC);

    P.AF = 0x0F;
    EXPECT_EQ(0, P.FZ);
    EXPECT_EQ(0, P.FN);
    EXPECT_EQ(0, P.FH);
    EXPECT_EQ(0, P.FC);

    P.AF = 0xF0;
    EXPECT_EQ(1, P.FZ);
    EXPECT_EQ(1, P.FN);
    EXPECT_EQ(1, P.FH);
    EXPECT_EQ(1, P.FC);
}

TEST(BitPacking, General) {
    P.AF = 0xFF00;
    EXPECT_EQ(0xFF, P.A);
    EXPECT_EQ(0x00, P.F);

    P.AF = 0x00FF;
    EXPECT_EQ(0x00, P.A);
    EXPECT_EQ(0xFF, P.F);

    P.BC = 0xFF00;
    EXPECT_EQ(0xFF, P.B);
    EXPECT_EQ(0x00, P.C);
    
    P.BC = 0x00FF;
    EXPECT_EQ(0x00, P.B);
    EXPECT_EQ(0xFF, P.C);

    P.DE = 0xFF00;
    EXPECT_EQ(0xFF, P.D);
    EXPECT_EQ(0x00, P.E);
    
    P.DE = 0x00FF;
    EXPECT_EQ(0x00, P.D);
    EXPECT_EQ(0xFF, P.E);

    P.HL = 0xFF00;
    EXPECT_EQ(0xFF, P.H);
    EXPECT_EQ(0x00, P.L);
    
    P.HL = 0x00FF;
    EXPECT_EQ(0x00, P.H);
    EXPECT_EQ(0xFF, P.L);
}

int main(int argc, char ** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
