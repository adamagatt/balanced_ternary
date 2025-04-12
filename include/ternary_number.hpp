#ifndef _BALANCED_TERNARY_HPP_
#define _BALANCED_TERNARY_HPP_

#include <algorithm>
#include <array>
#include <ostream>
#include <string_view>

#include "trit.hpp"

template <size_t N>
class TernaryNumber {
public:
    explicit TernaryNumber();
    explicit TernaryNumber(std::string_view encoded);

    auto operator==(const TernaryNumber<N>& rhs) const -> bool;

    auto operator-() const -> TernaryNumber<N>;

    auto operator+(const TernaryNumber<N>& rhs) const -> TernaryNumber<N>;
    auto operator+=(const TernaryNumber<N>& rhs);
    
    auto operator-(const TernaryNumber<N>& rhs) const -> TernaryNumber<N>;
    auto operator-=(const TernaryNumber<N>& rhs);
    
    auto operator*(const TernaryNumber<N>& rhs) const -> TernaryNumber<N>;
    
    auto operator<<(size_t positions) const -> TernaryNumber<N>;
    auto operator<<=(size_t positions);

    explicit operator int32_t() const;

    template <size_t M>
    friend std::ostream& operator<<(std::ostream& os, const TernaryNumber<M>& rhs);

private:
    std::array<Trit, N> value;
};

template <size_t N>
auto TernaryNumber<N>::operator-() const -> TernaryNumber<N> {
    TernaryNumber<N> out;

    std::transform(
        value.begin(), value.end(),
        out.value.begin(),
        invertTrit
    );

    return out;
}

template <size_t N>
TernaryNumber<N>::TernaryNumber() : value{Trit::ZERO} { }

template <size_t N>
TernaryNumber<N>::TernaryNumber(std::string_view encoded) : value{Trit::ZERO} {
    size_t length = std::min(N, encoded.size());

    std::transform(
        encoded.begin(), encoded.end(),
        std::next(value.begin(), N-length),
        tritFromEncoded
    );
}

template <size_t N>
auto TernaryNumber<N>::operator==(const TernaryNumber<N>& rhs) const -> bool {
    return value == rhs.value;
}

template <size_t N>
auto TernaryNumber<N>::operator+(const TernaryNumber<N>& rhs) const -> TernaryNumber<N> {
    TernaryNumber<N> out;
    
    SumResult sumResult{};

    // Ideally a scan operation but none currently exist that allow for
    // two input collections to be zipped together
    std::transform(
        value.rbegin(), value.rend(),
        rhs.value.rbegin(),
        out.value.rbegin(),
        [&sumResult](const Trit& lhs, const Trit& rhs) {
            sumResult = addTrits(lhs, rhs, sumResult.carry);
            return sumResult.result;
        }
    );

    return out;
}

template <size_t N>
auto TernaryNumber<N>::operator+=(const TernaryNumber<N>& rhs) {
    SumResult sumResult{};

    // Ideally a scan operation but none currently exist that allow for
    // two input collections to be zipped together
    std::transform(
        value.rbegin(), value.rend(),
        rhs.value.rbegin(),
        value.rbegin(),
        [&sumResult](const Trit& lhs, const Trit& rhs) {
            sumResult = addTrits(lhs, rhs, sumResult.carry);
            return sumResult.result;
        }
    );
}

template <size_t N>
auto TernaryNumber<N>::operator-(const TernaryNumber<N>& rhs) const -> TernaryNumber<N> {
    return *this + (-rhs);
}

template <size_t N>
auto TernaryNumber<N>::operator-=(const TernaryNumber<N>& rhs) {
    *this += (-rhs);
}

template <size_t N>
TernaryNumber<N>::operator int32_t() const {
    int32_t result = 0;

    for (auto val = 1, it = value.rbegin(); it != value.rend(); val*=3, ++it) {
        if (*it == Trit::POS) {
            result += val;
        } else if (*it == Trit::NEG) {
            result -= val;
        }
    }

    return result;
}

template <size_t N>
auto TernaryNumber<N>::operator*(const TernaryNumber<N>& rhs) const -> TernaryNumber<N> {
    TernaryNumber<N> out;

    for (auto it = value.rbegin(), rhs_shifted = rhs; it != value.rend(); ++it, rhs_shifted <<= 1) {
        if (*it == Trit::POS) {
            out += rhs_shifted;
        } else if (*it == Trit::NEG) {
            out -= rhs_shifted;
        }
    }

    return out;
}

template <size_t N>
auto TernaryNumber<N>::operator<<(size_t positions) const -> TernaryNumber<N> {
    if (positions >= N) {
        return TernaryNumber<N>();
    }

    TernaryNumber<N> out;

    std::copy(
        std::next(value.begin(), positions), value.end(),
        out.value.begin()
    );

    return out;
}

template <size_t N>
auto TernaryNumber<N>::operator<<=(size_t positions) {
    if (positions >= N) {
        std::fill(value.begin(), value.end(), Trit::ZERO);
        return;
    }

    std::rotate(value.begin(), std::next(value.begin(), positions), value.end());
    std::fill(value.rbegin(), std::next(value.rbegin(), positions), Trit::ZERO);
}

template <size_t M>
std::ostream& operator<<(std::ostream& os, const TernaryNumber<M>& rhs) {
    for (Trit trit : rhs.value) {
        if (trit == Trit::POS) {
            os << '+';
        } else if (trit == Trit::NEG) {
            os << '-';
        } else {
            os << '0';
        }
    }

    os << " (" << static_cast<int32_t>(rhs) << ")";

    return os;
}

#endif
