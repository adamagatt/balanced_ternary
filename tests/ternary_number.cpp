#include <gtest/gtest.h>

#include <sstream>
#include "ternary_number.hpp"

TEST(TernaryNumber, Output) {
    TernaryNumber<8> num_50 {"+-0--"}; // 50
    
    std::stringstream repr;
    repr << num_50;

    ASSERT_EQ(repr.str(), "000+-0-- (50)");
}

TEST (TernaryNumber, Inverse) {
    TernaryNumber<8> num_35{"++0-"}; // 35
    EXPECT_EQ(-num_35, TernaryNumber<8>{"--0+"}); // Negation is -35
    ASSERT_EQ(-(-num_35), TernaryNumber<8>{"++0-"}); // Double negation is 35
}

TEST (TernaryNumber, LeftShift) {
    TernaryNumber<8> num_neg_8{"-0+"}; // -8

    EXPECT_EQ(num_neg_8 << 1, TernaryNumber<8>{"0000-0+0"});
    EXPECT_EQ(num_neg_8 << 2, TernaryNumber<8>{"000-0+00"});
    EXPECT_EQ(num_neg_8 << 3, TernaryNumber<8>{"00-0+000"});
    EXPECT_EQ(num_neg_8 << 4, TernaryNumber<8>{"0-0+0000"});
    EXPECT_EQ(num_neg_8 << 5, TernaryNumber<8>{"-0+00000"});
    EXPECT_EQ(num_neg_8 << 6, TernaryNumber<8>{"0+000000"});
    EXPECT_EQ(num_neg_8 << 7, TernaryNumber<8>{"+0000000"});
    EXPECT_EQ(num_neg_8 << 8, TernaryNumber<8>{"00000000"});

    auto shifting_num = num_neg_8;
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, TernaryNumber<8>{"0000-0+0"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, TernaryNumber<8>{"000-0+00"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, TernaryNumber<8>{"00-0+000"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, TernaryNumber<8>{"0-0+0000"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, TernaryNumber<8>{"-0+00000"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, TernaryNumber<8>{"0+000000"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, TernaryNumber<8>{"+0000000"});
    shifting_num <<= 1;
    ASSERT_EQ(shifting_num, TernaryNumber<8>{"00000000"});
}

TEST(TernaryNumber, BinaryOperations) {
    TernaryNumber<8> num_23{"+0--"}; // 23
    TernaryNumber<8> num_33{"++-0"}; // 33

    EXPECT_EQ(num_23 + num_33, TernaryNumber<8>{"+-0+-"}); // Sum to 56
    EXPECT_EQ(num_23 - num_33, TernaryNumber<8>{"-0-"}); // Difference is -10
    EXPECT_EQ(num_33 - num_23, TernaryNumber<8>{"+0+"}); // Difference is 10
    EXPECT_EQ(num_23 * num_33, TernaryNumber<8>{"+00+0+0"}); // Product is 759

    SUCCEED();
}
