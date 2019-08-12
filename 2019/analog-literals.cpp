#include <iostream>
#include <cassert>

#include "analogliterals.hpp"

int main()
{
    using namespace analog_literals::symbols;

    unsigned int c = (   o-----o
                         |     !
                         !     !
                         !     !
                         o-----o ).area;

    assert(   ( o-----o
                |     !
                !     !
                !     !
                !     !
                o-----o ).area == ( o---------o
                                    |         !
                                    !         !
                                    o---------o ).area );

    std::cout << c << std::endl;

    return 0;
}
