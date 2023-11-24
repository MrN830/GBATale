#pragma once

#include <bn_assert.h>
#include <bn_math.h>

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

/**
 * @brief Return a normalized `bn::fixed_point` vector.
 */
inline auto normalized(const bn::fixed_point& v) -> bn::fixed_point
{
    const bn::fixed scale = bn::sqrt(v.x() * v.x() + v.y() * v.y());
    return {v.x() / scale, v.y() / scale};
}

} // namespace ut::util
