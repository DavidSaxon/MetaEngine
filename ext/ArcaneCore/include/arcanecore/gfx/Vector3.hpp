/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANECORE_GFX_VECTOR3_HPP_
#define ARCANECORE_GFX_VECTOR3_HPP_

#include "arcanecore/base/str/UTF8String.hpp"

namespace arc
{
namespace gfx
{

/*!
 * \brief Represents a 3 dimensional vector.
 */
class Vector3
{
public:

    //--------------------------------------------------------------------------
    //                             PUBLIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief X component of this Vector3.
     */
    float x;
    /*!
     * \brief Y component of this Vector3.
     */
    float y;
    /*!
     * \brief Z component of this Vector3.
     */
    float z;

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

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Default constructor.
     *
     * Creates a new Vector3 with the x, y, and z components initialized as 0.
     */
    Vector3();

    /*!
     * \brief Component constructor.
     *
     * Creates a new Vector3 with the given x, y, and z component values.
     */
    Vector3(float x, float y, float z);

    /*!
     * \brief Scalar constructor.
     *
     * Creates a new Vector3 with the x, y, and z components initialised to the
     * given scalar.
     */
    Vector3(float scalar);

    /*!
     * \brief Copy constructor
     *
     * Creates a new copy of the given Vector3.
     */
    Vector3(const Vector3& other);

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Assignment operator.
     *
     * Assigns the values of this Vector3 to be a copy of the values of the
     * other given Vector3.
     *
     * \param other Vector3 to copy data from.
     * \return A reference to this Vector3 once the assignment has taken place.
     */
    Vector3& operator=(const Vector3& other);

    /*!
     * \brief Equality operator.
     *
     * Compares whether this Vector3 and the other given Vector3 are considered
     * "equal". A comparison of the components of the vector is performed using
     * arc::math::float_equals using he default values for
     * ```delta_threshold``` and ```ulps_threshold```.
     *
     * \param other Vector3 to compare this vector against.
     * \return Whether the vectors are considered equal.
     */
    bool operator==(const Vector3& other) const;

    /*!
     * \brief Inequality operator.
     *
     * Compares this Vector3 and the other given Vector3 are not considered
     * "equal". See Vector3::operator== for further information.
     *
     * \param other Vector3 to compare this vector against.
     * \return Whether the vectors are not considered equal.
     */
    bool operator!=(const Vector3& other) const;

    /*!
     * \brief Inverse operator.
     *
     * \return A new Vector3 which is a copy of this Vector3 with its values
     * inverted.
     */
    Vector3 operator-() const;

    /*!
     * \brief Scalar addition operator.
     *
     * \param scalar The scalar to add to this Vector3.
     * \return A new Vector3 containing the results of the addition.
     */
    Vector3 operator+(float scalar) const;

    /*!
     * \brief Scalar compound addition operator.
     *
     * \param scalar The scalar to add to this Vector3.
     * \return A reference to this Vector3 once the addition has taken place.
     */
    Vector3& operator+=(float scalar);

    /*!
     * \brief Vector addition operator.
     *
     * \param other The other Vector3 to add to this Vector3
     * \return A new Vector3 containing the results of the addition.
     */
    Vector3 operator+(const Vector3& other) const;

    /*!
     * \brief Vector compound addition operator.
     *
     * \param other The other Vector3 to add to this Vector3.
     * \return A reference to this Vector3 once the addition has taken place.
     */
    Vector3& operator+=(const Vector3& other);

    /*!
     * \brief Scalar subtraction operator.
     *
     * \param scalar The scalar to subtract from this Vector3.
     * \return A new Vector3 containing the results of the subtraction.
     */
    Vector3 operator-(float scalar) const;

    /*!
     * \brief Scalar compound subtraction operator.
     *
     * \param scalar The scalar to subtract from this Vector3.
     * \return A reference to this Vector3 once the subtraction has taken place.
     */
    Vector3& operator-=(float scalar);

    /*!
     * \brief Vector subtraction operator.
     *
     * \param other The other Vector3 to subtract from this Vector3.
     * \return A new Vector3 containing the results of the subtraction.
     */
    Vector3 operator-(const Vector3& other) const;

    /*!
     * \brief Vector compound subtraction operator.
     *
     * \param other The other Vector3 to subtract from this Vector3.
     * \return A reference to this Vector3 once the subtraction has taken place.
     */
    Vector3& operator-=(const Vector3& other);

    /*!
     * \brief Scalar multiplication operator.
     *
     * \param scalar The scalar to multiply this Vector3 by.
     * \return A new vector containing the results of the multiplication.
     */
    Vector3 operator*(float scalar) const;

    /*!
     * \brief Scalar compound multiplication operator.
     *
     * \param scalar The scalar to multiply this Vector3 by.
     * \return A reference to this Vector3 once the multiplication has taken
     *         place.
     */
    Vector3& operator*=(float scalar);

    /*!
     * \brief Vector multiplication operator.
     *
     * \param other The other Vector3 to multiply this Vector3 by.
     * \return A new Vector3 containing the results of the multiplication.
     */
    Vector3 operator*(const Vector3& other) const;

    /*!
     * \brief Vector compound multiplication operator.
     *
     * \param other The other Vector3 to multiply this Vector3 by.
     * \return A reference to this Vector3 once the multiplication has taken
     *         place.
     */
    Vector3& operator*=(const Vector3& other);

    /*!
     * \brief Scalar division operator.
     *
     * \param scalar The scalar to divide this Vector3 by.
     * \return A new Vector3 containing the results of the division.
     */
    Vector3 operator/(float scalar) const;

    /*!
     * \brief Scalar compound division operator.
     *
     * \param scalar The scalar to divide this Vector3 by.
     * \return A reference to this Vector3 once the division has taken place.
     */
    Vector3& operator/=(float scalar);

    /*!
     * \brief Vector division operator.
     *
     * \param other The other Vector3 to divide this Vector3 by.
     * \return A new Vector3 containing the results of the division.
     */
    Vector3 operator/(const Vector3& other) const;

    /*!
     * \brief Vector compound division operator.
     *
     * \param other The other Vector3 to divide this Vector3 by.
     * \return A reference to this Vector3 once the division has taken
     *         place.
     */
    Vector3& operator/=(const Vector3& other);

};

//------------------------------------------------------------------------------
//                               EXTERNAL OPERATORS
//------------------------------------------------------------------------------

arc::str::UTF8String& operator<<(
        arc::str::UTF8String& s,
        const Vector3& v);

std::ostream& operator<<(std::ostream& stream, const Vector3& v);

} // namespace gfx
} // namespace arc

#endif
