/*!
 * \file
 * \brief Operations relating to Unicode string data.
 * \author David Saxon
 */
#ifndef CHAOSCORE_BASE_UNI_UNICODEOPERATIONS_HPP_
#define CHAOSCORE_BASE_UNI_UNICODEOPERATIONS_HPP_

#include "chaoscore/base/Types.hpp"
#include "chaoscore/base/data/BinaryOperations.hpp"
#include "chaoscore/base/uni/UTF8String.hpp"

namespace chaos
{
namespace uni
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Returns whether the given Unicode code point is a digit or not.
 *
 * Example usage:
 *
 * \code
 * chaos::uni::UTF8String s( "5" );
 * chaos::uni::is_digit( s.get_code_point( 0 ) ); // returns: true
 * \endcode
 *
 */
bool is_digit( chaos::uint32 code_point );

/*!
 * \brief Converts the given data stream of UTF-16 encoded text to new
 *        chaos::uni::UTF8String.
 *
 * \param data UTF-16 string stored as 2-4 chars per symbol.
 * \param symbol_length The number of Unicode symbols in the input data. Note:
 *                      this is not the number of bytes in the data. If this
 *                      parameter is set to chaos::uni::npos the length of the
 *                      input will be calculated with the assumption it is NULL
 *                      terminated.
 */
chaos::uni::UTF8String utf16_to_utf8(
        const char* data,
        std::size_t symbol_length,
        chaos::data::Endianness endianness = chaos::data::ENDIAN_LITTLE );

/*!
 * \brief Converts the given chaos::uni::UTF8String encoded data to a new c
 *        style string of UTF-16 encoded data.
 *
 * The resulting UTF-16 data will be null terminated with `0x00`, `0x00`.
 *
 * \warning This operation allocates a new char array that must be deleted.
 *
 * \param data UTF8String object to convert to a c style string of UTF-16
 *             encoded data.
 * \param r_length Returns the length of the new UTF-16 encoded string in bytes.
 * \param endianness The endian to use for the UTF-16 encoding, defaults to
 *                   little endian.
 * \return Newly allocated UTF-16 encoded string.
 */
char* utf8_to_utf16(
        const chaos::uni::UTF8String& data,
        std::size_t& r_length,
        chaos::data::Endianness endianness = chaos::data::ENDIAN_LITTLE );

/*!
 * \brief Joins the given vector into a single chaos::uni::UTF8String.
 *
 * The components of the vector will be concatenated together with the separator
 * string between each component.
 *
 * Example usage:
 *
 * \code
 * std::vector< chaos::uni::UTF8String > components;
 * components.push_back( "Hello" );
 * components.push_back( "World" );
 * chaos::uni::join( components, "_" ); returns: "Hello_World";
 * \endcode
 *
 * \param components Vector of string components to join together as a single
 *                   string.
 * \param separator chaos::uni::UTF8String to join each component together with.
 * \return New chaos::uni::UTF8String containing the results of the operation.
 */
chaos::uni::UTF8String join(
        const std::vector< chaos::uni::UTF8String >& components,
        const chaos::uni::UTF8String& separator );

} // namespace uni
} // namespace chaos

#endif
