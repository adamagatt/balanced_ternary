#ifndef _TRIT_HPP_
#define _TRIT_HPP_

#include <cstdint>

namespace BT {

/**
 * In balanced ternary a "trit" is a three-value digit that can have
 * a value of -1, 0 or 1.
 */
enum class Trit: int8_t {
    NEG = -1,
    ZERO = 0,
    POS = 1
};

/**
 * When calculating a sum we need to know the in-place result and the
 * carry, which is also a trit.
 */
struct SumResult {
    Trit result{};
    Trit carry{};

    /**
     * Returns true if two SumResults have the same result value and
     * the same carry value. This is largely used in testing, and needs
     * to be explicitly created to be supported by GTest.
     * @param rhs The other SumResult to compare against
     * @return true if the SumResults have the same content
     */
    auto operator==(const SumResult& rhs) const -> bool;
};

/**
 * Convert the character representing of a trit into a Trit enum
 * value. This representation accepts '+' as the +1 trit, '-1' as the
 * -1 trit and '0' as the zero trit. Any other characters are currently
 * parsed as 0.
 * 
 * @param encoded A character representing a trit
 * @return The trit represented by the submitted character, or the zero
 * trit if an invalid character is provided.
 */
auto tritFromEncoded(char encoded) -> Trit;

/**
 * Return the opposite of the submitted trit, i.e. '+1' is returned for
 * '-1' and vice versa. The negation of '0' is '0'.
 * 
 * @param trit A trit to find the negation for
 * @return The negation of the submitted trit
 */
auto negateTrit(const Trit& trit) -> Trit;

/**
 * A half-adder that returns the sum of two trits. The result is both
 * a direct value and potentially a carry trit that needs to be propagated
 * to the next trit when summing a full ternary number. 
 * 
 * @param t1 The first trit to add
 * @param t2 The second trit to add
 * @return The result and carry for adding the two trits
 */
auto addTrits(const Trit& t1, const Trit& t2) -> SumResult;

/**
 * A full-adder that sums three trits; usually matching-index trits from
 * two ternary numbers and the carry from the previous index. The carry is
 * not treated specially, but really is just a third trit to add. Returns
 * a result and carry trit similar to the binary addTrits() case.
 * 
 * @param t1 The first trit to add
 * @param t2 The second trit to add
 * @param carry A carry trit to also include in the addition 
 * @return The result and carry for adding the three trits
 */
auto addTrits(const Trit& t1, const Trit& t2, const Trit& carry) -> SumResult;

}

#endif