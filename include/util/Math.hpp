#pragma once

#include <bn_assert.h>

namespace ut::util
{

/**
 * @brief Floor divide operation, which gives floor results for negative dividend.
 * http://www.microhowto.info/howto/round_towards_minus_infinity_when_dividing_integers_in_c_or_c++.html
 */
template <typename Int>
Int div_floor(Int a, Int m)
{
    Int q = a / m;
    Int r = a % m;
    if ((r != 0) && ((r < 0) != (m < 0)))
        --q;
    return q;
}

/**
 * @brief Floor modulo operation, which gives positive results for negative dividend.
 * http://www.microhowto.info/howto/round_towards_minus_infinity_when_dividing_integers_in_c_or_c++.html
 */
template <typename Int>
Int mod_floor(Int a, Int m)
{
    Int r = a % m;
    if ((r != 0) && ((r < 0) != (m < 0)))
        r += m;
    return r;
}

} // namespace ut::util
