#include <gtest/gtest.h>

#include "trit.hpp"

TEST(Trit, PosAndNegCancelOut) {
    auto result = addTrits(Trit::POS, Trit::NEG);
    SumResult expected{.result = Trit::ZERO, .carry = Trit::ZERO};
    ASSERT_EQ(result, expected);
}

TEST(Trit, AddTwoPosResultsInCarry) {
    auto result = addTrits(Trit::POS, Trit::POS);
    SumResult expected{.result = Trit::NEG, .carry = Trit::POS};
    ASSERT_EQ(result, expected);
}

TEST(Trit, AddTwoNegResultsInCarry) {
    auto result = addTrits(Trit::NEG, Trit::NEG);
    SumResult expected{.result = Trit::POS, .carry = Trit::NEG};
    ASSERT_EQ(result, expected);
}

TEST(Trit, PosIsOppositeOfNeg) {
    ASSERT_EQ(negateTrit(Trit::POS), Trit::NEG);
    ASSERT_EQ(negateTrit(Trit::NEG), Trit::POS);
}

TEST(Trit, ZeroIsOwnNegative) {
    ASSERT_EQ(negateTrit(Trit::ZERO), Trit::ZERO);
}

TEST(Trit, DoubleNegationHasNoChange) {
    ASSERT_EQ(negateTrit(negateTrit(Trit::POS)), Trit::POS);
    ASSERT_EQ(negateTrit(negateTrit(Trit::NEG)), Trit::NEG);
}