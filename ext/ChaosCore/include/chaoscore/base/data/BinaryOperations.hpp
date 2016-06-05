/*!
 * \file
 * \brief functions for manipulating and reading binary data.
 * \author David Saxon
 */
#ifndef CHAOSCORE_BASE_DATA_BINARYOPERATIONS_HPP_
#define CHAOSCORE_BASE_DATA_BINARYOPERATIONS_HPP_

namespace chaos
{
namespace data
{

//------------------------------------------------------------------------------
//                                  ENUMERATORS
//------------------------------------------------------------------------------

/*!
 * \brief The possible endian types.
 */
enum Endianness
{
    /// The least-significant byte is stored at the highest memory address.
    ENDIAN_BIG = 0,
    /// The least-significant byte is stored at the lowest memory address.
    ENDIAN_LITTLE
};

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Returns the endianness of the current system this is running on.
 */
Endianness get_system_endianness();

} // namespace data
} // namespace chaos

#endif
