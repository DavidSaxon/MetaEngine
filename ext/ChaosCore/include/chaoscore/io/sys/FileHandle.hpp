/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSCORE_IO_SYS_FILEHANDLE_HPP_
#define CHAOSCORE_IO_SYS_FILEHANDLE_HPP_

#include "chaoscore/base/Types.hpp"
#include "chaoscore/io/sys/Path.hpp"

namespace chaos
{
namespace io
{
namespace sys
{

/*!
 * \brief Abstract base class used for representing an object that is writing or
 *        reading to/from a file.
 *
 * This object defines the specifications for chaos::io::sys::FileReader and
 * chaos::io::sys::FileWriter.
 */
class FileHandle
{
private:

    CHAOS_DISALLOW_COPY_AND_ASSIGN( FileHandle );

public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~FileHandle()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Opens the file handle to the internal path.
     *
     * \note This function is to be implemented by derived classes.
     *
     * \throws chaos::ex::StateError If this file handle is already open.
     *
     * \throws chaos::io::sys::InvalidPathError If the path cannot be opened
     */
    virtual void open() = 0;

    /*!
     * \brief Sets the path and opens this handle to it.
     *
     * This function is short hand for:
     *
     * \code
     * my_file_handle.set_path( m_path );
     * my_file_handle.open();
     * \endcode
     *
     * \throws chaos::ex::StateError If this file handle is already open.
     *
     ** \throws chaos::io::sys::InvalidPathError If the path cannot be opened
     */
    virtual void open( const chaos::io::sys::Path& path );

    /*!
     * \brief Closes this file handle.
     *
     * \note This function is to be implemented by derived classes.
     */
    virtual void close() = 0;

    /*!
     * \brief Returns the path being used by this file handle.
     */
    virtual const chaos::io::sys::Path& get_path() const;

    /*!
     * \brief Sets the path to be used by this file handle.
     *
     * The path may only be set if the file handle is not currently open.
     *
     * \throws chaos::ex::StateError If this file handle is open.
     */
    virtual void set_path( const chaos::io::sys::Path& path );

    /*!
     * \brief Returns the descriptor flags of the this file handle.
     */
    virtual chaos::uint32 get_flags() const;

    /*!
     * \brief Sets the descriptor flags to be used by this file handle.
     *
     * Flags can only be set if the file handle is not currently open.
     *
     * \throws chaos::ex::StateError If this file handle is open.
     */
    virtual void set_flags( chaos::uint32 flags );


protected:

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The file path this handle is currently using.
     */
    chaos::io::sys::Path m_path;

    /*!
     * \brief Descriptor flags of the file handle.
     */
    chaos::uint32 m_flags;

    /*!
     * \brief Whether the file handle is currently open or not.
     */
    bool m_open;

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Default super constructor.
     *
     * Super constructor used to create a new unopened file handle.
     *
     * \param flags Flags used to describe how the file handle should be opened.
     */
    FileHandle( chaos::uint32 flags = 0U );

    /*!
     * \brief Path super constructor.
     *
     * This does not open the File Handle. But the derived constructors for
     * chaos::io::sys::FileReader and chaos::io::sys::FileWriter will open the
     * file handle.
     *
     * \param path Path to the file to open.
     * \param flags Flags used to describe how the file handle should be opened.
     */
    FileHandle( const chaos::io::sys::Path& path, chaos::uint32 flags = 0U );
};

} // namespace sys
} // namespace io
} // namespace chaos

#endif
