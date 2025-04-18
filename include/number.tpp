#ifndef _NUMBER_TPP_
#define _NUMBER_TPP

#ifndef _NUMBER_HPP_
#error __FILE__ should only be included from number.hpp
#endif

template <size_t N>
BT::Number<N>::Number() { }

template <size_t N>
BT::Number<N>::Number(std::string_view encoded) {
    size_t length = std::min(N, encoded.size());

    // Populate lowest N trits with decoded characters
    std::ranges::transform(encoded, std::next(value.begin(), N-length), tritFromEncoded);
}

template <size_t N>
auto BT::Number<N>::operator==(const Number<N>& rhs) const -> bool {
    return value == rhs.value;
}

template <size_t N>
auto BT::Number<N>::operator!=(const Number<N>& rhs) const -> bool {
    return !(value == rhs.value);
}

template <size_t N>
auto BT::Number<N>::operator<(const Number<N>& rhs) const -> bool {
    // Due to Trit enum being backed by appropriate integral values, the
    // default std::array lexicographical comparison is suitable logic
    // for comparing entire balanced ternary numbers.
    return value < rhs.value;
}

template <size_t N>
auto BT::Number<N>::operator<=(const Number<N>& rhs) const -> bool {
    return !(rhs < *this);
}

template <size_t N>
auto BT::Number<N>::operator>(const Number<N>& rhs) const -> bool {
    return rhs < *this;
}

template <size_t N>
auto BT::Number<N>::operator>=(const Number<N>& rhs) const -> bool {
    return !(*this < rhs);
}

template <size_t N>
auto BT::Number<N>::operator-() const -> Number<N> {
    Number<N> out;

    // Flip all trits
    std::ranges::transform(value, out.value.begin(), negateTrit);

    return out;
}

template <size_t N>
auto BT::Number<N>::operator+(const Number<N>& rhs) const -> Number<N> {
    Number<N> out;

    // Ideally a scan operation but none currently exist that allow for
    // two input collections to be zipped together
    std::ranges::transform(
        value | std::views::reverse,
        rhs.value | std::views::reverse,
        out.value.rbegin(),
        // Return sum of trits and hold carry trit in accumulator to
        // propagate up to the next index. Mutable lambda to allow held
        // carry trit to be updated.
        [sumResult=SumResult{}](const Trit& lhs, const Trit& rhs) mutable {
            sumResult = addTrits(lhs, rhs, sumResult.carry);
            return sumResult.result;
        }
    );

    return out;
}

template <size_t N>
auto BT::Number<N>::operator+=(const Number<N>& rhs) {

    // Ideally a scan operation but none currently exist that allow for
    // two input collections to be zipped together
    std::ranges::transform(
        value | std::views::reverse,
        rhs.value | std::views::reverse,
        value.rbegin(),
        // Return sum of trits and hold carry trit in accumulator to
        // propagate up to the next index. Mutable lambda to allow held
        // carry trit to be updated.
        [sumResult=SumResult{}](const Trit& lhs, const Trit& rhs) mutable {
            sumResult = addTrits(lhs, rhs, sumResult.carry);
            return sumResult.result;
        }
    );
}

template <size_t N>
auto BT::Number<N>::operator-(const Number<N>& rhs) const -> Number<N> {
    return *this + (-rhs);
}

template <size_t N>
auto BT::Number<N>::operator-=(const Number<N>& rhs) {
    *this += (-rhs);
}

template <size_t N>
auto BT::Number<N>::operator*(const Number<N>& rhs) const -> Number<N> {
    Number<N> out;

    // Balanced ternary multiplication is a simple shift-and-adding operation
    // made easy as a -1 trit in the lhs current index only requires the rhs
    // to be negated (flip all trits) before adding. Without a useful enumerate()
    // function or elegant way to zip our array with std::iota() we fall back on
    // multiple-initialisation and update in a simple for loop to create and update
    // an iterator together with a left-shifting copy of the rhs value.
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
auto BT::Number<N>::operator<<(size_t positions) const -> Number<N> {
    // Early exit if we left-shift far enough that our number just becomes zero
    if (positions >= N) {
        return Number<N>();
    }

    Number<N> out;

    // Left shift is just copying the correct trits from our value to the
    // start of our zero-initialised output number 
    std::copy(
        std::next(value.begin(), positions), value.end(),
        out.value.begin()
    );

    return out;
}

template <size_t N>
auto BT::Number<N>::operator<<=(size_t positions) {
    // Early exit if we left-shift far enough that our number just becomes zero
    if (positions >= N) {
        std::ranges::fill(value, Trit::ZERO);
        return;
    }

    // An in-place left-shift is achieved by rotating our value array by the
    // specified number of positions and then zeroing out the least-significant
    // trits.
    std::rotate(value.begin(), std::next(value.begin(), positions), value.end());
    // Zeroing performed right-to-left for slightly easier math
    std::fill(value.rbegin(), std::next(value.rbegin(), positions), Trit::ZERO);
}

template <size_t N>
BT::Number<N>::operator int32_t() const {
    int32_t result = 0;

    // Without a useful enumerate() function (or way to elegantly zip collection
    // elements with std::iota()) we use multiple initialisation and update to
    // track an iterator to the current trit (right-to-left) as well as the
    // absolute value of that position (tripling for each index). We add or
    // subtract that value (or do nothing) based on the trit encountered. With
    // an enumerate() iterator adaptor we could instead calculate the value with
    // pow(3, idx), perhaps more wastefully depending on compiler optimisation.
    // We could also technically determine our index in-loop with std::distance
    // but somehow this seems more wasteful even though for an std::array with
    // a random-access iterator the calculation of distance is O(1).
    for (auto val = 1, it = value.rbegin(); it != value.rend(); val*=3, ++it) {
        if (*it == Trit::POS) {
            result += val;
        } else if (*it == Trit::NEG) {
            result -= val;
        }
    }

    return result;
}

template <size_t M>
auto operator<<(std::ostream& os, const BT::Number<M>& rhs) -> std::ostream& {
    for (BT::Trit trit : rhs.value) {
        if (trit == BT::Trit::POS) {
            os << '+';
        } else if (trit == BT::Trit::NEG) {
            os << '-';
        } else {
            os << '0';
        }
    }

    os << " (" << static_cast<int32_t>(rhs) << ")";

    return os;
}

#endif