#include "ternary_number.hpp"

#include <iostream>

int main() {
    TernaryNumber<8> mynum {"+0--"};
    TernaryNumber<8> mynum2{"++-0"};

    std::cout << mynum << std::endl;
    std::cout << mynum2 << std::endl;

    std::cout << (mynum + mynum2) << std::endl;
    std::cout << (mynum - mynum2) << std::endl;
    std::cout << (mynum * mynum2) << std::endl;

    return 0;
}