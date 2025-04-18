# Balanced Ternary Calcuator

[![License: CC BY-NC 4.0](https://licensebuttons.net/l/by-nc/4.0/88x31.png)](https://creativecommons.org/licenses/by-nc/4.0/)

Balanced Ternary Calculator by Adam Gatt is licensed under a [Creative Commons Attribution-NonCommercial 4.0 International License](https://creativecommons.org/licenses/by-nc/4.0/).

---

A calculator for representing integer values and performing operations using the [balanced ternary](https://en.wikipedia.org/wiki/Balanced_ternary) numeric representation system. Operations currently supported include:
* Addition, subtraction, multiplication, integer division
* Pre- and post- increment and decrement
* Comparison operators
* Left shifting and unary negation
* Conversion to int32_t
* Printable representation to output stream

Balanced ternary is a positional number system where each digit is a three-value "trit" that can hold a value of -1, 0 or 1. I represent these visually with the symbols `-`, `0` and `+` respectively (other notations use `0` and `1` with `T` representing -1).

This calculator allows for the representing of values with an arbitrary amount of trits, and then basic integer operations. Ternary values can be read from strings using the `-`/`0`/`+` notation and are output using that notation alongside their corresponding decimal value.

Ternary systems allow for denser representation of numbers where three-value trits can be reliably implemented, at the cost of operations needing to support an additional symbol. "Balanced" ternary, which balanced each trit around zero, allows for particularly elegant math with very simple implementations for negatives, subtraction and multiplication with greatly reduced use of carries and no need for a twos-complement equivalent for negative values.

This implementation is focused on clarity of logic rather than efficiency. This is exemplified by each "trit" taking up a full byte when arguably only 2 bits are required and so packing could be employed.