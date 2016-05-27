/*!
 * \file
 * \brief Operations relating to string formatting.
 * author David Saxon
 */
#ifndef CHAOSCORE_IO_FORMATOPERATIONS_HPP_
#define CHAOSCORE_IO_FORMATOPERATIONS_HPP_

#include <iomanip>
#include <sstream>

#include "chaoscore/base/str/UTF8String.hpp"

namespace chaos
{
namespace io
{
namespace format
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Converts the given integer type to a chaos::str::UTF8String binary
 *        representation.
 *
 * \warning If the input `value` is not an integer type (int, long,
 *          chaos::uint32, chaos::int64, etc) this function may return
 *          unexpected results.
 *
 * The `zero_pad` parameter controls whether the resulting string will be padded
 * with `0` digits so that it has the maximum number of bits for the input type.
 *
 * Example of using the zero_pad functionality:
 *
 * \code
 * chaos::int32 i = 34892347;
 * chaos::io::format::int_to_binary( i );
 * // returns: "0b00000010000101000110101000111011"
 * \endcode
 *
 */
template< typename T >
chaos::str::UTF8String int_to_binary( T value, bool zero_pad = true )
{
    // returned string which will hold the binary result
    chaos::str::UTF8String u( "0b" );

    std::size_t bit_count = sizeof( T ) * 8;
    // use bitwise mask to evaluate whether each bit is a one or a zero
    // chaos::uint64 mask = 1 << ( bit_count - 1 );
    chaos::uint64 v = static_cast< chaos::uint64 >( value );
    bool non_zero_found = false;
    for ( std::size_t i = 0; i < bit_count; ++i )
    {
        chaos::uint64 mask = 1ULL << ( bit_count - 1 - i );

        if ( ( v & mask ) == 0 )
        {
            if( zero_pad || non_zero_found || i == bit_count - 1 )
            {
                u << "0";
            }
        }
        else
        {
            non_zero_found = true;
            u << "1";
        }
    }

    return u;
}

/*!
 * \brief Converts the given integer type to a chaos::str::UTF8String
 *        hexadecimal representation.
 *
 * \warning If the input `value` is not an integer type (int, long,
 *          chaos::uint32, chaos::int64, etc) this function may return
 *          unexpected results.
 *
 * The `zero_pad` parameter controls whether the resulting string will be padded
 * with `0` digits so that it has the maximum number of digits for the input
 * type.
 *
 * Example of using the zero_pad functionality:
 *
 * \code
 * chaos::int32 i = 3425;
 * chaos::io::format::int_to_hex( i );
 * // returns: "0x00000D61"
 * \endcode
 */
template< typename T >
chaos::str::UTF8String int_to_hex( T value, bool zero_pad = true )
{
    std::stringstream ss;
    ss << "0x";
    if ( zero_pad )
    {
        ss << std::setfill( '0' ) << std::setw( sizeof( T ) * 2 );
    }
    ss << std::uppercase << std::hex << value;

    return chaos::str::UTF8String( ss.str().c_str() );
}

/*!
 * \brief Centres the given text with whitespace on either side so that has a
 *        symbol length equal to line_length.
 *
 * If the symbol length of the text is greater than line_length then the text
 * will be left unmodified.
 *
 * \param text The text to be centered.
 * \param line_length The desired number of symbols in the line after this
 *                    operation has taken place.
 * \param trim_trailing whether trailing whitespace should be not be added.
 */
void centre_text(
        chaos::str::UTF8String& text,
        const chaos::uint32     line_length,
        bool                    trim_trailing = false );

} // namespace format
} // namespace io
} // namespace chaos

#endif
