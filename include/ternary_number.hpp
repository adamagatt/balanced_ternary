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

// As a fully templated class we can't use a separate translation unit
// compiled from a .cpp file; all our member function definitions have
// to be inline. This means we could have all of the definitions here
// in this file and make it hundreds of lines, or we can use a seperate
// template-implmentation file (often .tpp or .ipp) and simply include it
// at this point.
#include "ternary_number.tpp"

#endif
