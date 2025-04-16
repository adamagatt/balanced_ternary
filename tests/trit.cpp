#include <gtest/gtest.h>

#include "trit.hpp"

TEST(Trit, PosAndNegCancelOut) {
    auto result = BT::addTrits(BT::Trit::POS, BT::Trit::NEG);
    BT::SumResult expected{.result = BT::Trit::ZERO, .carry = BT::Trit::ZERO};
    ASSERT_EQ(result, expected);
}

TEST(Trit, AddTwoPosResultsInCarry) {
    auto result = BT::addTrits(BT::Trit::POS, BT::Trit::POS);
    BT::SumResult expected{.result = BT::Trit::NEG, .carry = BT::Trit::POS};
    ASSERT_EQ(result, expected);
}

TEST(Trit, AddTwoNegResultsInCarry) {
    auto result = BT::addTrits(BT::Trit::NEG, BT::Trit::NEG);
    BT::SumResult expected{.result = BT::Trit::POS, .carry = BT::Trit::NEG};
    ASSERT_EQ(result, expected);
}

TEST(Trit, PosIsOppositeOfNeg) {
    ASSERT_EQ(BT::negateTrit(BT::Trit::POS), BT::Trit::NEG);
    ASSERT_EQ(BT::negateTrit(BT::Trit::NEG), BT::Trit::POS);
}

TEST(Trit, ZeroIsOwnNegative) {
    ASSERT_EQ(BT::negateTrit(BT::Trit::ZERO), BT::Trit::ZERO);
}

TEST(Trit, DoubleNegationHasNoChange) {
    ASSERT_EQ(BT::negateTrit(negateTrit(BT::Trit::POS)), BT::Trit::POS);
    ASSERT_EQ(BT::negateTrit(negateTrit(BT::Trit::NEG)), BT::Trit::NEG);
}