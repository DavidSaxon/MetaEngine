/*!
 * \file
 * \brief functions for manipulating and reading byte data.
 * \author David Saxon
 */

#ifndef CHAOSCORE_BASE_DATA_BYTEOPERATIONS_HPP_
#define CHAOSCORE_BASE_DATA_BYTEOPERATIONS_HPP_

#include <cstdlib>

#include "chaoscore/base/Types.hpp"
#include "chaoscore/base/data/BinaryOperations.hpp"

namespace chaos
{
namespace data
{

/*!
 * \brief Converts an array of bytes to a single unsigned 32-bit integer.
 *
 * \param bytes Array of bytes to convert.
 * \param length The number of bytes in the data array provided.
 * \param endianness The endianness to use for the result chaos::uint32.
 *
 * Example usage:
 *
 * \code
 * const unsigned char data[ 4 ] = { 0xAE, 0x72, 0x8B, 0x10 };
 * chaos::data::bytes_to_uint32( data, 4 ); // returns 2926742288
 * \endcode
 *
 * \throws chaos::ex::ConversionDataError If the `length` parameter is greater
 *                                        than `sizeof( chaos::uint32 ) (4).
 *                                        Since data loss will occur attempting
 *                                        to pack more than 4 bytes in to a
 *                                        32-bit integer.`
 */
chaos::uint32 bytes_to_uint32(
        const void* bytes,
        std::size_t length,
        Endianness endianness = chaos::data::get_system_endianness() );

} // namespace data
} // namespace chaos

#endif
