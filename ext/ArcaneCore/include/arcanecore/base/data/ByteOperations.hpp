/*!
 * \file
 * \brief functions for manipulating and reading byte data.
 * \author David Saxon
 */

#ifndef ARCANECORE_BASE_DATA_BYTEOPERATIONS_HPP_
#define ARCANECORE_BASE_DATA_BYTEOPERATIONS_HPP_

#include <cstdlib>

#include "arcanecore/base/Types.hpp"
#include "arcanecore/base/data/BinaryOperations.hpp"

namespace arc
{
namespace data
{

/*!
 * \brief Converts an array of bytes to a single unsigned 32-bit integer.
 *
 * \param bytes Array of bytes to convert.
 * \param length The number of bytes in the data array provided.
 * \param endianness The endianness to use for the result arc::uint32.
 *
 * Example usage:
 *
 * \code
 * const unsigned char data[4] = {0xAE, 0x72, 0x8B, 0x10};
 * arc::data::bytes_to_uint32(data, 4); // returns 2926742288
 * \endcode
 *
 * \throws arc::ex::ConversionDataError If the `length` parameter is greater
 *                                        than `sizeof(arc::uint32) (4).
 *                                        Since data loss will occur attempting
 *                                        to pack more than 4 bytes in to a
 *                                        32-bit integer.`
 */
arc::uint32 bytes_to_uint32(
        const void* bytes,
        std::size_t length,
        Endianness endianness = arc::data::get_system_endianness());

} // namespace data
} // namespace arc

#endif
