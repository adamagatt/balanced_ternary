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

TEST(Trit, PosIsInverseOfNeg) {
    ASSERT_EQ(invertTrit(Trit::POS), Trit::NEG);
    ASSERT_EQ(invertTrit(Trit::NEG), Trit::POS);
}

TEST(Trit, ZeroIsOwnInverse) {
    ASSERT_EQ(invertTrit(Trit::ZERO), Trit::ZERO);
}

TEST(Trit, DoubleInverseHasNoChange) {
    ASSERT_EQ(invertTrit(invertTrit(Trit::POS)), Trit::POS);
    ASSERT_EQ(invertTrit(invertTrit(Trit::NEG)), Trit::NEG);
}