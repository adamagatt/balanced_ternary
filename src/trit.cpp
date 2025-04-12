#include "trit.hpp"

auto tritFromEncoded(char encoded) -> Trit {
    switch (encoded) {
        case '+':
            return Trit::POS;
        case '-':
            return Trit::NEG;
        default:
            return Trit::ZERO;
    }
}

auto invertTrit(const Trit& trit) -> Trit {
    switch (trit) {
        case Trit::POS:
            return Trit::NEG;
        case Trit::NEG:
            return Trit::POS;
    };

    return Trit::ZERO;
}

auto addTrits(const Trit& t1, const Trit& t2) -> SumResult {
    if (t1 == Trit::ZERO) {
        return {t2, Trit::ZERO};
    } else if (t2 == Trit::ZERO) {
        return {t1, Trit::ZERO};
    } else if (t1 == invertTrit(t2)) {
        return {Trit::ZERO, Trit::ZERO};
    } else { // Both Trits are the same
        return {invertTrit(t1), t1}; // Carry trit is needed
    }
}

auto addTrits(const Trit& t1, const Trit& t2, const Trit& t3) -> SumResult {
    if (t1 == Trit::ZERO) {
        return addTrits(t2, t3);
    } else if (t2 == Trit::ZERO) {
        return addTrits(t1, t3);
    } else if (t3 == Trit::ZERO) {
        return addTrits(t1, t2);
    } else if (invertTrit(t1) == t2) {
        return {t3, Trit::ZERO};
    } else if (invertTrit(t1) == t3) {
        return {t2, Trit::ZERO};
    } else if (invertTrit(t2) == t3) {
        return {t1, Trit::ZERO};
    } else { // All three trits are the same
        return {Trit::ZERO, t1};
    }
}