#ifndef _TRIT_HPP_
#define _TRIT_HPP_

#include <cstdint>

enum class Trit: int8_t {
    NEG = -1,
    ZERO = 0,
    POS = 1
};

struct SumResult {
    Trit result{};
    Trit carry{};
};

auto tritFromEncoded(char encoded) -> Trit;

auto invertTrit(const Trit& trit) -> Trit;

auto addTrits(const Trit& t1, const Trit& t2) -> SumResult;

auto addTrits(const Trit& t1, const Trit& t2, const Trit& t3) -> SumResult;

#endif