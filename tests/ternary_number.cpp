#include <gtest/gtest.h>

#include <sstream>
#include "ternary_number.hpp"

TEST(TernaryNumber, Output) {
    TernaryNumber<8> num_50 {"+-0--"};
    
    std::stringstream repr;
    repr << num_50;

    ASSERT_EQ(repr.str(), "000+-0-- (50)");
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