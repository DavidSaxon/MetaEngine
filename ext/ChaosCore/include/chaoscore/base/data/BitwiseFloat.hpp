#ifndef CHAOSCORE_BASE_DATA_BITWISEFLOAT_HPP_
#define CHAOSCORE_BASE_DATA_BITWISEFLOAT_HPP_

#include "chaoscore/base/Types.hpp"

namespace chaos
{
namespace data
{

/*!
 * \brief Object that can be used to read and write the bits of a floating point
 *        number.
 *
 * Floating point numbers are composed of three sections: sign, exponent, and
 * mantissa. These sections are laid out like so:
 *
 * \code
 * 0 00000000 00000000000000000000000
 * ^ \------/ \---------------------/
 * |     |                |
 * \ 8-bit exponent       |
 *  \                23-bit mantissa
 * sign bit
 * \endcode
 *
 * This object provides functions for reading and writing these sections of a
 * floating point number.
 */
union BitwiseFloat
{
public:

    //--------------------------------------------------------------------------
    //                             PUBLIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Access to the float representation.
     */
    float float_rep;
    /*!
     * \brief Access to the int representation.
     */
    chaos::uint32 int_rep;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Float constructor.
     *
     * Creates a new BitewiseFloat initialised with the given value.
     */
    BitwiseFloat( float value );

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the other given BitwiseFloat.
     */
    BitwiseFloat( const BitwiseFloat& other );

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Assignment operator.
     *
     * Assigns the value of this BitwiseFloat to be a copy of the value of the
     * other given BitwiseFloat.
     *
     * \param other BitwiseFloat to copy from.
     * \return A reference to this BitwiseFloat once the assignment has taken
     *         place.
     */
    BitwiseFloat& operator=( const BitwiseFloat& other );

    /*!
     * \brief Equality operator.
     *
     * Compares whether this BitwiseFloat and the other BitwiseFloat are
     * considered equal.
     *
     * Equality is defined by a comparison of the exact BitwiseFloat.int_rep
     * values.
     *
     * For more flexible floating point comparisons see
     * chaos::math::float_equals().
     */
    bool operator==( const BitwiseFloat& other ) const;

    /*!
     * \brief Inequality operator.
     *
     * Compares whether this BitwiseFloat and the other BitwiseFloat are
     * considered not equal.
     *
     * Equality is defined by a comparison of the exact BitwiseFloat.int_rep
     * values.
     *
     * For more flexible floating point comparisons see
     * chaos::math::float_equals().
     */
    bool operator!=( const BitwiseFloat& other ) const;

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Retrieves the sign bit of this floating point number.
     */
    bool get_sign_bit() const;

    /*!
     * \brief Sets the sign bit (most significant bit) of this float.
     */
    void set_sign_bit( bool sign );

    /*!
     * \brief Retrieves the 8-bit exponent section of this floating point
     *        number.
     */
    chaos::uint32 get_exponent() const;

    /*!
     * \brief Sets the 8-bit exponent section of this floating point number.
     */
    void set_exponent( chaos::uint8 exponent );

    /*!
     * \brief Retrieves the 23-bit mantissa section of this floating point
     *        number.
     */
    chaos::uint32 get_mantissa() const;

    /*!
     * \brief Sets the 23-bit mantissa section of this floating point number.
     *
     * \note While a 32-bit input value is accepted only the least significant
     *       23-bits of this value will be used.
     */
    void set_mantissa( chaos::uint32 mantissa );

    /*!
     * \brief Returns the precision of this float away from zero.
     *
     * Floating point precision is measured as the difference between this
     * number and the next possible float value away from zero.
     *
     * \throws chaos::ex::ArithmeticError If this float is infinity or NaN.
     */
    float precision_away_from_zero() const;

    /*!
     * \brief Returns the precision of this float away from zero.
     *
     * Floating point precision is measured as the difference between this
     * number and the next possible float value towards zero.
     *
     * \throws chaos::ex::ArithmeticError If this float is 0 or NaN.
     */
    float precision_towards_zero() const;
};

} // namespace data
} // namespace chaos

#endif
