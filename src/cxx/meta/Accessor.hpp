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

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the file system path this Accessor is managing.
     */
    const chaos::io::sys::Path& get_path() const;

    /*!
     * \brief Returns whether this Accessor points to a directory or an actual
     *        data file.
     *
     * \returns True if this Accessor points to a directory.
     */
    bool is_directory() const;

    /*!
     * \brief Return the names of the components under this accessors.
     */
    const std::vector<chaos::uni::UTF8String>& get_components() const;

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The file or directory path this Accessor is reading.
     */
    const chaos::io::sys::Path m_path;
    /*!
     * \brief Stores whether this Accessor points to a directory or not at the
     *        time it was created.
     */
    const bool m_is_directory;

    /*!
     * \brief The list of components under this Accessors.
     */
    std::vector<chaos::uni::UTF8String> m_components;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Initialises this Accessor as a directory.
     */
    void directory_init();

    /*!
     * \breief Initialises this Accessor as a configuration file.
     */
    void file_init();
};

} // namespace meta

#endif
