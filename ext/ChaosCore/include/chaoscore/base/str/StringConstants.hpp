/*!
 * \file
 * \brief Global constants relating to Unicode data.
 * \author David Saxon
 */
#ifndef CHAOSCORE_BASE_STR_STRINGCONSTANTS_HPP_
#define CHAOSCORE_BASE_STR_STRINGCONSTANTS_HPP_

#include <cstddef>

#include "chaoscore/base/Types.hpp"

namespace chaos
{
namespace str
{

//------------------------------------------------------------------------------
//                                  ENUMERATORS
//------------------------------------------------------------------------------

// TODO: DELETE ME
/*!
 * \brief Encoding types for string data.
 */
enum Encoding
{
    ENCODING_ASCII = 0,
    ENCODING_UTF8,
    ENCODING_UTF16_LITTLE_ENDIAN,
    ENCODING_UTF16_BIG_ENDIAN
};

//------------------------------------------------------------------------------
//                                   VARIABLES
//------------------------------------------------------------------------------

// TODO: should this be capital?
/*!
 * \brief Value used to signify an invalid Unicode based index.
 */
extern const std::size_t npos;

/*!
 * \brief The number of Bytes in the UTF-8 Byte Order Marker.
 */
extern const std::size_t UTF8_BOM_SIZE;
/*!
 * \brief Bytes representing the UTF-8 Byte Order Marker.
 */
extern const char UTF8_BOM[];

/*!
 * \brief The number of Bytes in the UTF-16 Byte Order Marker.
 */
extern const std::size_t UTF16_BOM_SIZE;
/*!
 * \brief Bytes representing the Little Endian UTF-16 Byte Order Marker.
 */
extern const char UTF16LE_BOM[];
/*!
 * \brief Bytes representing the Big Endian UTF-16 Byte Order Marker.
 */
extern const char UTF16BE_BOM[];

/*!
 * \brief The maximum code point that will be stored within 2 bytes when encoded
 *        using UTF-16.
 */
extern const chaos::uint32 UTF16_MAX_2BYTE;
/*!
 * \brief Value to be subtracted from code points in order to produce the 20-bit
 *        number used to calculate the surrogate pairs of a 4-byte UTF-16
 *        encoded symbol.
 */
extern const chaos::uint32 UTF16_4BYTE_OFFSET;
/*!
 * \brief The minimum value for a 4-byte encoded UTF-16 symbol's high surrogate.
 */
extern const chaos::uint32 UTF16_HIGH_SURROGATE_MIN;
/*!
 * \brief The maximum value for a 4-byte encoded UTF-16 symbol's high surrogate.
 */
extern const chaos::uint32 UTF16_HIGH_SURROGATE_MAX;
/*!
 * \brief The minimum value for a 4-byte encoded UTF-16 symbol's low surrogate.
 */
extern const chaos::uint32 UTF16_LOW_SURROGATE_MIN;

} // namespace str
} // namespace chaos

#endif
