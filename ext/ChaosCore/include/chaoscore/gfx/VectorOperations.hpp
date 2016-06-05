/*!
 * \file
 * \brief Operations relating to vectors.
 * \author David Saxon
 */
#ifndef CHAOSCORE_GFX_VECTOROPERATIONS_HPP_
#define CHAOSCORE_GFX_VECTOROPERATIONS_HPP_

#include "chaoscore/gfx/Vector2.hpp"
#include "chaoscore/gfx/Vector3.hpp"
#include "chaoscore/gfx/Vector4.hpp"

namespace chaos
{
namespace gfx
{

/*!
 * \brief Calculates the magnitude of the given Vector2.
 */
float magnitude( const Vector2& v );

/*!
 * \brief Calculates the magnitude of the given Vector3.
 */
float magnitude( const Vector3& v );

/*!
 * \brief Calculates the magnitude of the given Vector4.
 */
float magnitude( const Vector4& v );

/*!
 * \brief Normalises the given Vector2.
 */
void normalise( Vector2& v );

/*!
 * \brief Normalises the given Vector3.
 */
void normalise( Vector3& v );

/*!
 * \brief Normalises the given Vector4.
 */
void normalise( Vector4& v );

/*!
 * |brief Calculate the dot product of the given Vector2s
 */
float dot_product( const Vector2& v_1, const Vector2& v_2 );

/*!
 * |brief Calculate the dot product of the given Vector3s
 */
float dot_product( const Vector3& v_1, const Vector3& v_2 );

/*!
 * |brief Calculate the dot product of the given Vector4s
 */
float dot_product( const Vector4& v_1, const Vector4& v_2 );

} // namespace gfx
} // namespace chaos

#endif
