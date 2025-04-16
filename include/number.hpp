#ifndef _NUMBER_HPP_
#define _NUMBER_HPP_

#include <algorithm>
#include <array>
#include <ostream>
#include <ranges>
#include <string_view>

#include "trit.hpp"

namespace BT {

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
class Number {
public:
    /**
     * Construct a new Ternary Number, defaulting to a value of zero.
     */
    Number();

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
    explicit Number(std::string_view encoded);

    /**
     * Determines if the submitted ternary number has the same value.
     * 
     * @param rhs Another ternary number to compare against.
     * @return true if the supplied number has the same value, false otherwise
     */
    auto operator==(const Number<N>& rhs) const -> bool;

    /**
     * Unary negation of the ternary number, where every trit simply has
     * its value flipped.
     * 
     * @return The unary negation of this ternary number 
     */
    auto operator-() const -> Number<N>;

    /**
     * Sum this ternary number against another that has been provided. This
     * may currently result in an overflow if the sum of the two numbers
     * requires a length that is greater than the templated size N.
     * 
     * @param rhs The number to add this number to
     * @return the result of adding this ternary number to the submitted
     * number.
     */
    auto operator+(const Number<N>& rhs) const -> Number<N>;

    /**
     * In-place addition of another ternary number into this one, modifying
     * this number rather than returning the sum. This may currently result
     * in an overflow if the sum of the two numbers requires a length that
     * is greater than the templated size N.
     * 
     * @param rhs The number to add into this number
     */
    auto operator+=(const Number<N>& rhs);
    
    /**
     * Return the result of subtracting another ternary number from this one.
     * This may currently result in an underflow if the result of the
     * subtraction requires a length that is greater than the templated size
     * N.
     * 
     * @param rhs The number to subtract from this one
     * @return the result of subtracting the submitted ternary number from
     * this one.
     */
    auto operator-(const Number<N>& rhs) const -> Number<N>;

    /**
     * In-place subtraction of another ternary number from this one, modifying
     * this number rather than returning the difference. This may currently
     * result in an underflow if the difference of the two numbers requires a
     * length that is greater than the templated size N.
     * 
     * @param rhs The number to subtract from this number
     */
    auto operator-=(const Number<N>& rhs);
    
    /**
     * Calculate the product of this ternary number multiplied with another
     * that has been provided. This may currently result in an overflow or
     * underflow if the product of the two numbers requires a length that is
     * greater than the templated size N.
     * 
     * @param rhs The number to multiply this number with
     * @return the product of this ternary number and the submitted number.
     */
    auto operator*(const Number<N>& rhs) const -> Number<N>;
    
    /**
     * Return the result of left-shifting this number by a specified amount
     * of trit positions. As each trit is explicitly signed this operation
     * is always a signed shift. This has the usual effect of multiplying the
     * number by 3. An over- or under-flow can occur if a non-zero most
     * significant trit is shifted, and this can potentially change the sign
     * of the result.
     *  
     * @param positions The amount of trits to shift the number by
     * @return The result of left-shifting this number by the specified number
     * of trit positions.
     */
    auto operator<<(size_t positions) const -> Number<N>;

    /**
     * In-place left-shift operation of this number by a specified amount
     * of trit positions. As each trit is explicitly signed this operation
     * is always a signed shift. This has the usual effect of multiplying the
     * number by 3. An over- or under-flow can occur if a non-zero most
     * significant trit is shifted, and this can potentially change the sign
     * of this number.
     *  
     * @param positions The amount of trits to shift this number by
     */  
    auto operator<<=(size_t positions);

    /**
     * The value of this number in traditional signed 32-bit representation.
     * 
     * @return This number in signed 32-bit representation
     */
    explicit operator int32_t() const;

    /**
     * Render a representation of this number to an output stream. This will
     * take the form of the number as an encoded sequence (using "-", "0" and
     * "+" characters), followed by its numerical value in brackets.
     * 
     * @tparam M Parameter required for a friend function implemented for a
     * templated class, named differently from N solely to prevent shadowing.
     * @param os The output stream to render this number's representation to
     * @param rhs The balanced ternary number to render to the output stream
     * @return std::ostream& The output stream again, for operation chaining
     */
    template <size_t M>
    friend auto operator<<(std::ostream& os, const Number<M>& rhs) -> std::ostream&;

private:
    // A balanced ternary number is a fixed-length sequence of trits
    std::array<Trit, N> value;
};

// As a fully templated class we can't use a separate translation unit
// compiled from a .cpp file; all our member function definitions have
// to be inline. This means we could have all of the definitions here
// in this file and make it hundreds of lines, or we can use a seperate
// template-implmentation file (often .tpp or .ipp) and simply include it
// at this point.
#include "number.tpp"

}

#endif
