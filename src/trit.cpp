#include "trit.hpp"

auto BT::SumResult::operator==(const SumResult& rhs) const -> bool {
    return result == rhs.result && carry == rhs.carry;
}

auto BT::tritFromEncoded(char encoded) -> Trit {
    switch (encoded) {
        case '+':
            return Trit::POS;
        case '-':
            return Trit::NEG;
        default:
            return Trit::ZERO;
    }
}

auto BT::negateTrit(Trit trit) -> Trit {
    switch (trit) {
        case Trit::POS:
            return Trit::NEG;
        case Trit::NEG:
            return Trit::POS;
    };

    return Trit::ZERO;
}

auto BT::addTrits(Trit t1, Trit t2) -> SumResult {
    // If either trit is zero the sum is just the other trit
    if (t1 == Trit::ZERO) {
        return {t2, Trit::ZERO};
    } else if (t2 == Trit::ZERO) {
        return {t1, Trit::ZERO};
    } else if (t1 == negateTrit(t2)) {
        // If one trit is the negation of the other the sum is zero
        return {Trit::ZERO, Trit::ZERO};
    } else {
        // If both Trits are the same we need a carry trit
        return {negateTrit(t1), t1};
    }
}

auto BT::addTrits(Trit t1, Trit t2, Trit carry) -> SumResult {
    // If any trit is zero we can reduce to the binary sum
    if (t1 == Trit::ZERO) {
        return addTrits(t2, carry);
    } else if (t2 == Trit::ZERO) {
        return addTrits(t1, carry);
    } else if (carry == Trit::ZERO) {
        return addTrits(t1, t2);
    }
    
    // If any two trits negate each other the remaining trit is the reuslt
    if (negateTrit(t1) == t2) {
        return {carry, Trit::ZERO};
    } else if (negateTrit(t1) == carry) {
        return {t2, Trit::ZERO};
    } else if (negateTrit(t2) == carry) {
        return {t1, Trit::ZERO};
    }
    
    // Else all three trits are the same, so the result is zero with a
    // carry trit
    return {Trit::ZERO, t1};
}