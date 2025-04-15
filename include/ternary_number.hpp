#ifndef _BALANCED_TERNARY_HPP_
#define _BALANCED_TERNARY_HPP_

#include <algorithm>
#include <array>
#include <ostream>
#include <ranges>
#include <string_view>

#include "trit.hpp"

/**
 * A number in ternary is an array of trit values, similar to how a binary
 * encoding is an array of bits. This is an implementation of a number in
 * "Balanced Ternary", a system where each trit can be "-1", "0" or "+1".
 * 
 * This is a templated class to allow the user to specify the number of trits
 * to use for the number. All binary operations only support operating on
 * numbers that share the same size.
 * 
 * @tparam N The number of trits to use in the number.
 */
template <size_t N>
class TernaryNumber {
public:
    /**
     * Construct a new Ternary Number, defaulting to a value of zero.
     */
    TernaryNumber();

    /**
     * Construct a new Ternary Number with a value provided in the
     * specified character input. If the provided encoded value is shorter
     * than the templated length then the number is left-padded with zero-
     * trits. If it is longer then it is truncated and only the N right-most
     * characters are used.
     * 
     * @param encoded An encoding of the value to initialise the ternary
     * number with, where '-' represents -1, '+' represents +1 and '0'
     * represents zero.
     */
    explicit TernaryNumber(std::string_view encoded);

    /**
     * Determines if the submitted ternary number has the same value.
     * 
     * @param rhs Another ternary number to compare against.
     * @return true if the supplied number has the same value, false otherwise
     */
    auto operator==(const TernaryNumber<N>& rhs) const -> bool;

    /**
     * Unary negation of the ternary number, where every trit simply has
     * its value flipped.
     * 
     * @return The unary negation of this ternary number 
     */
    auto operator-() const -> TernaryNumber<N>;

    /**
     * Sum this ternary number against another that has been provided. This
     * may currently result in an overflow if the sum of the two numbers
     * requires a length that is greater than the templated size N.
     * 
     * @param rhs The number to add this number to
     * @return the result of adding this ternary number to the submitted
     * number.
     */
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
TernaryNumber<N>::TernaryNumber() {
    std::ranges::fill(value, Trit::ZERO);
}

template <size_t N>
TernaryNumber<N>::TernaryNumber(std::string_view encoded) {
    size_t length = std::min(N, encoded.size());

    std::ranges::fill(value, Trit::ZERO);
    std::ranges::transform(encoded, std::next(value.begin(), N-length), tritFromEncoded);
}

template <size_t N>
auto TernaryNumber<N>::operator-() const -> TernaryNumber<N> {
    TernaryNumber<N> out;

    std::ranges::transform(value, out.value.begin(), negateTrit);

    return out;
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
    std::ranges::transform(
        value | std::views::reverse,
        rhs.value | std::views::reverse,
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
    std::ranges::transform(
        value | std::views::reverse,
        rhs.value | std::views::reverse,
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
