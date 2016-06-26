/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANECORE_GFX_VECTOR4_HPP_
#define ARCANECORE_GFX_VECTOR4_HPP_

#include "arcanecore/base/str/UTF8String.hpp"

namespace arc
{
namespace gfx
{

/*!
 * \brief Represents a 4 dimensional vector.
 */
class Vector4
{
public:

    //--------------------------------------------------------------------------
    //                             PUBLIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief X component of this Vector4.
     */
    float x;
    /*!
     * \brief Y component of this Vector4.
     */
    float y;
    /*!
     * \brief Z component of this Vector4.
     */
    float z;
    /*!
     * \brief W component of this Vector4.
     */
    float w;

    /*!
     * \brief R access component of this vector (Alias of the X component).
     */
    float& r;
    /*!
     * \brief G access component of this vector (Alias of the Y component).
     */
    float& g;
    /*!
     * \brief B access component of this vector (Alias of the X component).
     */
    float& b;
    /*!
     * \brief A access component of this vector (Alias of the w component).
     */
    float& a;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Default constructor.
     *
     * Creates a new Vector4 with the x, y, z, and w components initialised as
     * 0.
     */
    Vector4();

    /*!
     * \brief Component constructor.
     *
     * Creates a new Vector4 with the given x, y, z, and w component values.
     */
    Vector4(float x, float y, float z, float w);

    /*!
     * \brief Scalar constructor.
     *
     * Creates a new Vector4 with the x, y, z, and w components to the given
     * scalar.
     */
    Vector4(float scalar);

    /*!
     * \brief Copy constructor.
     *
     * Creates a new copy of the given Vector4.
     */
    Vector4(const Vector4& other);


    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Assignment operator.
     *
     * Assigns the values of this Vector4 to be a copy of the values of the
     * other given Vector4.
     *
     * \param other Vector4 to copy data from.
     * \return A reference to this Vector4 once the assignment has taken place.
     */
    Vector4& operator=(const Vector4& other);

    /*!
     * \brief Equality operator.
     *
     * Compares whether this Vector4 and the other given Vector4 are considered
     * "equal". A comparison of the component of the vector is performed using
     * arc::math::float_equals using the default values for
     * ```delta_threshold``` and ```ulps_threshold```.
     *
     * \param other Vector4 to compare this vector against.
     * \return Whether the vectors are considered equal.
     */
    bool operator==(const Vector4& other) const;

    /*!
     * \brief Inequality operator.
     *
     * Compares this Vector4 and the other given Vector4 are not considered
     * "equal". See Vector4::operator== for further information.
     *
     * \param other Vector4 to compare this vector against.
     * \return Whether the vectors are not considered equal.
     */
    bool operator!=(const Vector4& other) const;

    /*!
     * \brief Inverse operator.
     *
     * \return A new Vector4 which is a copy of this Vector4 with its values
     * inverted.
     */
    Vector4 operator-() const;

    /*!
     * \brief Scalar addition operator.
     *
     * \param scalar The scalar to add to this Vector4.
     * \return A new Vector4 containing the results of the addition.
     */
    Vector4 operator+(float scalar) const;

    /*!
     * \brief Scalar compound addition operator.
     *
     * \param scalar The scalar to add to this Vector4.
     * \return A reference to this Vector4 once the addition has taken place.
     */
    Vector4& operator+=(float scalar);

    /*!
     * \brief Vector addition operator.
     *
     * \param other The other Vector4 to add to this Vector4
     * \return A new Vector4 containing the results of the addition.
     */
    Vector4 operator+(const Vector4& other) const;

    /*!
     * \brief Vector compound addition operator.
     *
     * \param other The other Vector4 to add to this Vector4.
     * \return A reference to this Vector4 once the addition has taken place.
     */
    Vector4& operator+=(const Vector4& other);

    /*!
     * \brief Scalar subtraction operator.
     *
     * \param scalar The scalar to subtract from this Vector4.
     * \return A new Vector4 containing the results of the subtraction.
     */
    Vector4 operator-(float scalar) const;

    /*!
     * \brief Scalar compound subtraction operator.
     *
     * \param scalar The scalar to subtract from this Vector4.
     * \return A reference to this Vector4 once the subtraction has taken place.
     */
    Vector4& operator-=(float scalar);

    /*!
     * \brief Vector subtraction operator.
     *
     * \param other The other Vector4 to subtract from this Vector4.
     * \return A new Vector4 containing the results of the subtraction.
     */
    Vector4 operator-(const Vector4& other) const;

    /*!
     * \brief Vector compound subtraction operator.
     *
     * \param other The other Vector4 to subtract from this Vector4.
     * \return A reference to this Vector4 once the subtraction has taken place.
     */
    Vector4& operator-=(const Vector4& other);

    /*!
     * \brief Scalar multiplication operator.
     *
     * \param scalar The scalar to multiply this Vector4 by.
     * \return A new vector containing the results of the multiplication.
     */
    Vector4 operator*(float scalar) const;

    /*!
     * \brief Scalar compound multiplication operator.
     *
     * \param scalar The scalar to multiply this Vector4 by.
     * \return A reference to this Vector4 once the multiplication has taken
     *         place.
     */
    Vector4& operator*=(float scalar);

    /*!
     * \brief Vector multiplication operator.
     *
     * \param other The other Vector4 to multiply this Vector4 by.
     * \return A new Vector4 containing the results of the multiplication.
     */
    Vector4 operator*(const Vector4& other) const;

    /*!
     * \brief Vector compound multiplication operator.
     *
     * \param other The other Vector4 to multiply this Vector4 by.
     * \return A reference to this Vector4 once the multiplication has taken
     *         place.
     */
    Vector4& operator*=(const Vector4& other);

    /*!
     * \brief Scalar division operator.
     *
     * \param scalar The scalar to divide this Vector4 by.
     * \return A new Vector4 containing the results of the division.
     */
    Vector4 operator/(float scalar) const;

    /*!
     * \brief Scalar compound division operator.
     *
     * \param scalar The scalar to divide this Vector4 by.
     * \return A reference to this Vector4 once the division has taken place.
     */
    Vector4& operator/=(float scalar);

    /*!
     * \brief Vector division operator.
     *
     * \param other The other Vector4 to divide this Vector4 by.
     * \return A new Vector4 containing the results of the division.
     */
    Vector4 operator/(const Vector4& other) const;

    /*!
     * \brief Vector compound division operator.
     *
     * \param other The other Vector4 to divide this Vector4 by.
     * \return A reference to this Vector4 once the division has taken
     *         place.
     */
    Vector4& operator/=(const Vector4& other);
};

//------------------------------------------------------------------------------
//                               EXTERNAL OPERATORS
//------------------------------------------------------------------------------

arc::str::UTF8String& operator<<(
        arc::str::UTF8String& s,
        const Vector4& v);

std::ostream& operator<<(std::ostream& stream, const Vector4& v);

} // namespace gfx
} // namespace arc

#endif
