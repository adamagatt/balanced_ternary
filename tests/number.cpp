#include <gtest/gtest.h>
#include "number.hpp"

#include <cstdlib>
#include <sstream>

TEST(Number, OutputRepresentation) {
    const BT::Number<8> num_50 {"+-0--"};
    
    std::stringstream repr;
    repr << num_50;

    EXPECT_EQ(repr.str(), "000+-0-- (50)");
}

TEST (Number, Comparisons) {
    const BT::Number<8>& num_0 = BT::Number<8>::ZERO;
    const BT::Number<8> num_17{"+-0-"};
    const BT::Number<8> num_17_copy{"+-0-"};
    const BT::Number<8> num_neg_17{"-+0+"};

    EXPECT_EQ(num_17, num_17_copy);

    EXPECT_NE(num_0, num_17);
    EXPECT_NE(num_17, num_neg_17);

    EXPECT_LT(num_0, num_17);
    EXPECT_LT(num_neg_17, num_0);
    EXPECT_LT(num_neg_17, num_17);

    EXPECT_GT(num_17, num_neg_17);
    EXPECT_GT(num_0, num_neg_17);
    EXPECT_GT(num_17, num_0);

    EXPECT_LE(num_0, num_17);
    EXPECT_LE(num_neg_17, num_0);
    EXPECT_LE(num_neg_17, num_17);
    EXPECT_LE(num_17, num_17_copy);

    EXPECT_GE(num_17, num_neg_17);
    EXPECT_GE(num_0, num_neg_17);
    EXPECT_GE(num_17, num_0);
    ASSERT_GE(num_17, num_17_copy);
}

TEST (Number, Increments) {
    const BT::Number<8> num_neg_one{"-"};
    const BT::Number<8>& num_0 = BT::Number<8>::ZERO;
    const BT::Number<8> num_one{"+"};    

    // Pre-increment provides the incremented value
    auto temp = num_neg_one;
    EXPECT_EQ(++temp, num_0);
    
    temp = num_0;
    EXPECT_EQ(++temp, num_one);

    temp = num_neg_one;
    EXPECT_EQ(++(++temp), num_one);

    // Post-increment provides the original value
    temp = num_neg_one;
    EXPECT_EQ(temp++, num_neg_one);

    temp = num_0;
    EXPECT_EQ(temp++, num_0);

    temp = num_one;
    EXPECT_EQ(temp++, num_one);

    // Test a chain of carries
    BT::Number<8> num_neg_14{"-+++"};
    EXPECT_EQ(++num_neg_14, BT::Number<8>{"0---"}); // -14 + 1 = -13
}

TEST (Number, Decrements) {
    const BT::Number<8> num_neg_one{"-"};
    const BT::Number<8>& num_zero = BT::Number<8>::ZERO; 
    const BT::Number<8> num_one{"+"};    

    // Pre-decrement provides the decremented value
    auto temp = num_zero;
    EXPECT_EQ(--temp, num_neg_one);

    temp = num_one;
    EXPECT_EQ(--temp, num_zero);

    temp = num_one;
    EXPECT_EQ(--(--temp), num_neg_one);

    // Post-decrement provides the original value
    temp = num_neg_one;
    EXPECT_EQ(temp--, num_neg_one);

    temp = num_zero;
    EXPECT_EQ(temp--, num_zero);

    temp = num_one;
    EXPECT_EQ(temp--, num_one);

    // Test a chain of carries
    BT::Number<8> num_14{"+---"};
    EXPECT_EQ(--num_14, BT::Number<8>{"0+++"}); // 14 - 1 = 13
}

TEST (Number, UnaryNegation) {
    const BT::Number<8> num_35{"++0-"};
    EXPECT_EQ(-num_35, BT::Number<8>{"--0+"}); // Negation is -35
    EXPECT_EQ(-(-num_35), BT::Number<8>{"++0-"}); // Double negation is 35

    // Only one representation of zero, and so negative zero is still zero
    EXPECT_EQ(-BT::Number<8>::ZERO, BT::Number<8>::ZERO);
}

TEST (Number, LeftShift) {
    const BT::Number<8> num_neg_8{"-0+"};

    EXPECT_EQ(num_neg_8 << 1, BT::Number<8>{"0000-0+0"});
    EXPECT_EQ(num_neg_8 << 2, BT::Number<8>{"000-0+00"});
    EXPECT_EQ(num_neg_8 << 3, BT::Number<8>{"00-0+000"});
    EXPECT_EQ(num_neg_8 << 4, BT::Number<8>{"0-0+0000"});
    EXPECT_EQ(num_neg_8 << 5, BT::Number<8>{"-0+00000"});
    EXPECT_EQ(num_neg_8 << 6, BT::Number<8>{"0+000000"});
    EXPECT_EQ(num_neg_8 << 7, BT::Number<8>{"+0000000"});
    EXPECT_EQ(num_neg_8 << 8, BT::Number<8>{"00000000"});
}

TEST (Number, InPlaceLeftShift) {
    BT::Number<8> shifting_num{"-0+"}; // -8
    
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, BT::Number<8>{"0000-0+0"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, BT::Number<8>{"000-0+00"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, BT::Number<8>{"00-0+000"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, BT::Number<8>{"0-0+0000"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, BT::Number<8>{"-0+00000"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, BT::Number<8>{"0+000000"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, BT::Number<8>{"+0000000"});
    shifting_num <<= 1;
    EXPECT_EQ(shifting_num, BT::Number<8>{"00000000"});
}

TEST(Number, BinaryOperations) {
    const BT::Number<8> num_23{"+0--"};
    const BT::Number<8> num_33{"++-0"};

    EXPECT_EQ(num_23 + num_33, BT::Number<8>{"+-0+-"}); // Sum to 56
    EXPECT_EQ(num_23 - num_33, BT::Number<8>{"-0-"}); // Difference is -10
    EXPECT_EQ(num_33 - num_23, BT::Number<8>{"+0+"}); // Difference is 10
    EXPECT_EQ(num_23 * num_33, BT::Number<8>{"+00+0+0"}); // Product is 759
}

TEST(Number, IntegerDivision) {
    const BT::Number<8> num_59{"+-+--"};
    const BT::Number<8> num_60{"+-+-0"};
    const BT::Number<8> num_61{"+-+-+"};
    const BT::Number<8> num_12{"++0"};

    // Integral division with remainders discarded
    EXPECT_EQ(num_59 / num_12, BT::Number<8>{"0++"}); // 59 / 12 = 4
    EXPECT_EQ(num_60 / num_12, BT::Number<8>{"+--"}); // 60 / 12 = 5
    EXPECT_EQ(num_61 / num_12, BT::Number<8>{"+--"}); // 61 / 12 = 5

    // Negatively signed numerators and divisors, results rounded towards zero
    EXPECT_EQ(-num_59 /  num_12, BT::Number<8>{"0--"}); // -59 /  12 = -4
    EXPECT_EQ( num_59 / -num_12, BT::Number<8>{"0--"}); //  59 / -12 = -4
    EXPECT_EQ(-num_59 / -num_12, BT::Number<8>{"0++"}); // -59 / -12 =  4

    // Dividing zero by any number results in zero
    const auto& num_0 = BT::Number<8>::ZERO;

    EXPECT_EQ(num_0 / num_60, num_0);    // 0 /  60 = 0
    EXPECT_EQ(num_0 / (-num_60), num_0); // 0 / -60 = 0

    // Dividing by zero currently kills the program
    EXPECT_EXIT(
        num_61 / num_0,
        testing::ExitedWithCode(EXIT_FAILURE),
        "Attempt to divide by zero"
    );

    EXPECT_EXIT(
        (-num_61) / num_0,
        testing::ExitedWithCode(EXIT_FAILURE),
        "Attempt to divide by zero"
    );
}

TEST(Number, InPlaceBinaryOperations) {
    const BT::Number<8> num_23{"+0--"};
    const BT::Number<8> num_33{"++-0"};

    auto temp = num_23;
    temp += num_33;
    EXPECT_EQ(temp, BT::Number<8>{"+-0+-"}); // Sum to 56

    temp = num_23;
    temp -= num_33;
    EXPECT_EQ(temp, BT::Number<8>{"-0-"}); // Difference is -10
    
    temp = num_33;
    temp -= num_23;
    EXPECT_EQ(temp, BT::Number<8>{"+0+"}); // Difference is 10
    
    temp = num_23;
    temp *= num_33;
    EXPECT_EQ(temp, BT::Number<8>{"+00+0+0"}); // Product is 759
}