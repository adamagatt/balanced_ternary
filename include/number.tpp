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

    std::ranges::transform(encoded, std::next(value.begin(), N-length), tritFromEncoded);
}

template <size_t N>
auto BT::Number<N>::operator==(const Number<N>& rhs) const -> bool {
    return value == rhs.value;
}

template <size_t N>
auto BT::Number<N>::operator-() const -> Number<N> {
    Number<N> out;

    std::ranges::transform(value, out.value.begin(), negateTrit);

    return out;
}

template <size_t N>
auto BT::Number<N>::operator+(const Number<N>& rhs) const -> Number<N> {
    Number<N> out;
    
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
auto BT::Number<N>::operator+=(const Number<N>& rhs) {
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
    if (positions >= N) {
        return Number<N>();
    }

    Number<N> out;

    std::copy(
        std::next(value.begin(), positions), value.end(),
        out.value.begin()
    );

    return out;
}

template <size_t N>
auto BT::Number<N>::operator<<=(size_t positions) {
    if (positions >= N) {
        std::fill(value.begin(), value.end(), Trit::ZERO);
        return;
    }

    std::rotate(value.begin(), std::next(value.begin(), positions), value.end());
    std::fill(value.rbegin(), std::next(value.rbegin(), positions), Trit::ZERO);
}

template <size_t N>
BT::Number<N>::operator int32_t() const {
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