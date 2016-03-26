/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_ACCESSOR_HPP_
#define METAENGINE_ACCESSOR_HPP_

#include <chaoscore/base/Preproc.hpp>
#include <chaoscore/io/sys/Path.hpp>

namespace meta
{

/*!
 * \brief TODO
 *
 * TODO
 */
class Accessor
{
private:

    CHAOS_DISALLOW_COPY_AND_ASSIGN(Accessor);

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    Accessor(const chaos::io::sys::Path& path);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The file or directory path this Accessor is reading.
     */
    chaos::io::sys::Path m_path;
};

} // namespace meta

#endif
