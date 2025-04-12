#include <gtest/gtest.h>

#include <iostream>
#include "ternary_number.hpp"


TEST(TernaryNumber, Main) {
    TernaryNumber<8> mynum {"+0--"};
    TernaryNumber<8> mynum2{"++-0"};

    std::cout << mynum << std::endl;
    std::cout << mynum2 << std::endl;

    std::cout << (mynum + mynum2) << std::endl;
    std::cout << (mynum - mynum2) << std::endl;
    std::cout << (mynum * mynum2) << std::endl;

    SUCCEED();
}