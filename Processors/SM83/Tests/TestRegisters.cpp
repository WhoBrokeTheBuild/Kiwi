#include <gtest/gtest.h>
#include <cstdio>

#include <Kiwi/SM83/SM83.hpp>

using namespace kiwi::SM83;

Registers R;

TEST(BitPacking, Flags) {
    R.F = 0x00;
    EXPECT_EQ(0, R.FZ);
    EXPECT_EQ(0, R.FN);
    EXPECT_EQ(0, R.FH);
    EXPECT_EQ(0, R.FC);

    R.F = 0xFF;
    EXPECT_EQ(1, R.FZ);
    EXPECT_EQ(1, R.FN);
    EXPECT_EQ(1, R.FH);
    EXPECT_EQ(1, R.FC);

    R.F = 0b01010000;
    EXPECT_EQ(1, R.FZ);
    EXPECT_EQ(0, R.FN);
    EXPECT_EQ(1, R.FH);
    EXPECT_EQ(0, R.FC);

    R.F = 0b10101111;
    EXPECT_EQ(0, R.FZ);
    EXPECT_EQ(1, R.FN);
    EXPECT_EQ(0, R.FH);
    EXPECT_EQ(1, R.FC);

    R.AF = 0x0F;
    EXPECT_EQ(0, R.FZ);
    EXPECT_EQ(0, R.FN);
    EXPECT_EQ(0, R.FH);
    EXPECT_EQ(0, R.FC);

    R.AF = 0xF0;
    EXPECT_EQ(1, R.FZ);
    EXPECT_EQ(1, R.FN);
    EXPECT_EQ(1, R.FH);
    EXPECT_EQ(1, R.FC);
}

TEST(BitPacking, General) {
    R.AF = 0xFF00;
    EXPECT_EQ(0xFF, R.A);
    EXPECT_EQ(0x00, R.F);

    R.AF = 0x00FF;
    EXPECT_EQ(0x00, R.A);
    EXPECT_EQ(0xFF, R.F);

    R.BC = 0xFF00;
    EXPECT_EQ(0xFF, R.B);
    EXPECT_EQ(0x00, R.C);
    
    R.BC = 0x00FF;
    EXPECT_EQ(0x00, R.B);
    EXPECT_EQ(0xFF, R.C);

    R.DE = 0xFF00;
    EXPECT_EQ(0xFF, R.D);
    EXPECT_EQ(0x00, R.E);
    
    R.DE = 0x00FF;
    EXPECT_EQ(0x00, R.D);
    EXPECT_EQ(0xFF, R.E);

    R.HL = 0xFF00;
    EXPECT_EQ(0xFF, R.H);
    EXPECT_EQ(0x00, R.L);
    
    R.HL = 0x00FF;
    EXPECT_EQ(0x00, R.H);
    EXPECT_EQ(0xFF, R.L);
}

int main(int argc, char ** argv)
{
    return RUN_ALL_TESTS();
}
