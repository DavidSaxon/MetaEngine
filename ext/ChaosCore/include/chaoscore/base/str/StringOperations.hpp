/*!
 * \file
 * \brief Operations relating to string data.
 * \author David Saxon
 */
#ifndef CHAOSCORE_BASE_STR_STRINGOPERATIONS_HPP_
#define CHAOSCORE_BASE_STR_STRINGOPERATIONS_HPP_

#include "chaoscore/base/Types.hpp"
#include "chaoscore/base/data/BinaryOperations.hpp"
#include "chaoscore/base/str/UTF8String.hpp"

namespace chaos
{
namespace str
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
 * chaos::str::UTF8String s("5");
 * chaos::str::is_digit(s.get_code_point(0)); // returns: true
 * \endcode
 *
 */
bool is_digit(chaos::uint32 code_point);

/*!
 * \brief Converts the given data stream of UTF-16 encoded text to new
 *        chaos::str::UTF8String.
 *
 * \param data UTF-16 string stored as 2-4 chars per symbol.
 * \param byte_length The number of bytes in the input data. If this parameter
 *                      is set to chaos::str::npos the length of the input will
 *                      be calculated with the assumption it is null terminated.
 */
chaos::str::UTF8String utf16_to_utf8(
        const char* data,
        std::size_t byte_length,
        chaos::data::Endianness endianness = chaos::data::ENDIAN_LITTLE);

/*!
 * \brief Converts the given chaos::str::UTF8String encoded data to a new c
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
 * \param null_terminated Whether the returned string should end with the UTF-16
 *                        null terminator (0x00, 0x00).
 *
 * \return Newly allocated UTF-16 encoded string.
 */
char* utf8_to_utf16(
        const chaos::str::UTF8String& data,
        std::size_t& r_length,
        chaos::data::Endianness endianness = chaos::data::ENDIAN_LITTLE,
        bool null_terminated = true);

/*!
 * \brief Checks whether the given character data is valid UTF-8 encoded data.
 *
 * UTF-8 encoding expects that symbols match one of these patterns:
 *
 * - One byte symbol: `0xxxxxxx`
 * - Two byte symbol: `110xxxxx 10xxxxxx`
 * - Three byte symbol: `1110xxxx 10xxxxxx 10xxxxxx`
 * - Four byte symbol: `11110xxx 10xxxxxx 10xxxxxx 10xxxxxx`
 *
 * \param length The length of that data provided, if set to chaos::str::npos
 *        (default) it will be assumed that the character data ends with a null
 *        terminator.
 *
 * \return Whether the data is UTF-8 encoded.
 */
bool is_utf8(const char* data, std::size_t length = chaos::str::npos);

/*!
 * \brief Joins the given vector into a single chaos::str::UTF8String.
 *
 * The components of the vector will be concatenated together with the separator
 * string between each component.
 *
 * Example usage:
 *
 * \code
 * std::vector<chaos::str::UTF8String> components;
 * components.push_back("Hello");
 * components.push_back("World");
 * chaos::str::join(components, "_"); returns: "Hello_World";
 * \endcode
 *
 * \param components Vector of string components to join together as a single
 *                   string.
 * \param separator chaos::str::UTF8String to join each component together with.
 * \return New chaos::str::UTF8String containing the results of the operation.
 */
chaos::str::UTF8String join(
        const std::vector<chaos::str::UTF8String>& components,
        const chaos::str::UTF8String& separator);

} // namespace str
} // namespace chaos

#endif
