/*!
 * \file
 * \brief Operations relating to math.
 * \author David Saxon
 */
#ifndef ARCANECORE_BASE_MATHOPERATIONS_HPP_
#define ARCANECORE_BASE_MATHOPERATIONS_HPP_

#include <cfloat>

#include "arcanecore/base/math/MathConstants.hpp"

namespace arc
{
namespace math
{

/*!
 * \brief Checks whether two floating point values are equal or almost equal.
 *
 * This performs a two stage check:
 *
 * - First the values are subtracted from one another and if the absolute result
 *   is less than or equal to ```delta_threshold``` then this operation resolves
 *   true.
 *
 * - Second the integer representations of the values are subtracted from one
 *   another and if the absolute result is less than or equal to
 *   ```ulps_threshold``` then this function resolves true.
 *
 * The first check is performed for the purpose of values near 0 where ULPs
 * style comparisons break down, e.g. ```0.0F == -0.0F```.
 *
 * The second comparison is a comparison of the difference of units in last
 * place (ULPs) between the two floats. This effectively compares the number
 * of possible float representations between the two values. Larger numbers with
 * less precision will have a bigger numerical difference than two smaller
 * values with more precision for the same ULPs difference.
 *
 * \param a The first float to compare.
 * \param b The second float to compare.
 * \param delta_threshold If ```a``` and ```b``` have a difference less than
 *                        or equal to this, they are considered equal.
 * \param ulps_threshold If ```a``` and ```b``` have units in last place
 *                       difference less than or equal to this, they are
 *                       considered equal.
 */
bool float_equals(
        float a,
        float b,
        float delta_threshold = FLT_EPSILON,
        arc::uint32 ulps_threshold = 8);

} // namespace math
} // namespace arc

#endif
