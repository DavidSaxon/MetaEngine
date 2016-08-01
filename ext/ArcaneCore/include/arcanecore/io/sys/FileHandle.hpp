/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANECORE_IO_SYS_FILEHANDLE_HPP_
#define ARCANECORE_IO_SYS_FILEHANDLE_HPP_

#include "arcanecore/base/Types.hpp"
#include "arcanecore/io/sys/Path.hpp"

namespace arc
{
namespace io
{
namespace sys
{

/*!
 * \brief Abstract base class that represents an object used for providing
 *        access to a file.
 */
class FileHandle
{
private:

    ARC_DISALLOW_COPY_AND_ASSIGN(FileHandle);

public:

    //--------------------------------------------------------------------------
    //                                 ENUMERATOR
    //--------------------------------------------------------------------------

    /*!
     * \brief The possible encodings for FileHandles.
     */
    enum Encoding
    {
        /// The FileHandle will attempt to detect the encoding of the file.
        ENCODING_DETECT = 0,
        /// The file is pure single byte data, this represents binary or ASCII
        /// files.
        ENCODING_RAW,
        /// The file is encoded in UTF-8 Unicode.
        ENCODING_UTF8,
        /// The file is encoded in little endian UTF-16 Unicode.
        ENCODING_UTF16_LITTLE_ENDIAN,
        /// The file is encoded in big endian UTF-16 Unicode.
        ENCODING_UTF16_BIG_ENDIAN
    };

    /*!
     * \brief The possible newline symbols for FileHandles to use.
     */
    enum Newline
    {
        /// Line endings will be detected based on the current operating system.
        NEWLINE_DETECT = 0,
        /// Unix style newlines will be used: ```'\n'```.
        NEWLINE_UNIX,
        /// Windows style newlines will be used: ```'\r\n'```.
        NEWLINE_WINDOWS
    };

    //--------------------------------------------------------------------------
    //                              MOVE CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Move constructor.
     *
     * \param other The FileHandle to move resources from.
     */
    FileHandle(FileHandle&& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~FileHandle()
    {
    }

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Move assignment operator.
     *
     * Moves resources from the given FileHandle to this FileHandle.
     *
     * \param other The FileHandle to move resources from.
     */
    FileHandle& operator=(FileHandle&& other);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns whether the FileHandle is currently open or not.
     */
    bool is_open() const;

    /*!
     * \brief Returns the path being used by this FileHandle.
     */
    const arc::io::sys::Path& get_path() const;

    /*!
     * \brief Returns the encoding being used by this FileHandle.
     */
    Encoding get_encoding() const;

    /*!
     * \brief Returns the newline symbol being used by this FileHandle.
     */
    Newline get_newline() const;

    /*!
     * \brief Sets the path to be used by this FileHandle.
     *
     * \note The path can only be set if this FileHandle is not currently open.
     *
     * \throws arc::ex::StateError If this FileHandle is open.
     */
    void set_path(const arc::io::sys::Path& path);

    /*!
     * \brief Sets the encoding to be used by this FileHandle.
     *
     * \note The encoding can only be set if this FileHandle is not currently
     *       open.
     *
     * \throws arc::ex::StateError If this FileHandle is open.
     */
    void set_encoding(Encoding encoding);

    /*!
     * \brief Sets the newline symbol to be used by this FileHandle.
     *
     * \note The newline symbol can only be set if this FileHandle is not
     *       currently open.
     *
     * \throws arc::ex::StateError If this FileHandle is open.
     */
    void set_newline(Newline newline);

    /*!
     * \brief Opens the FileHandle to the internal path.
     *
     * \throws arc::ex::StateError If this FileHandle is already open.
     * \throws arc::ex::IOError If the path cannot be opened.
     */
    virtual void open() = 0;

    /*!
     * \brief Sets the path and opens the FileHandle to the path.
     *
     * This function is shorthand for:
     *
     * \code
     * my_file_handle.set_path(my_path);
     * my_file_handle.open();
     * \endcode
     *
     * \param path The path to the file to use.
     *
     * \throws arc::ex::StateError If this FileHandle is already open.
     * \throws arc::ex::IOError If the path cannot be opened.
     */
    virtual void open(const arc::io::sys::Path& path);

    /*!
     * \brief Closes this FileHandle.
     */
    virtual void close() = 0;

    /*!
     * \brief Returns the size of the file in bytes.
     *
     * \throws arc::ex::StateError If the FileHandle is not open.
     */
    virtual arc::int64 get_size() const = 0;

    /*!
     * \brief Returns the size in bytes of the Byte Order Marker that should be
     *        used in this file based on the encoding type.
     *
     * \note This function does not imply that the file actually has a BOM.
     */
    std::size_t get_bom_size() const;

    /*!
     * \brief Returns the index of the byte the file position indicator is
     *        currently at.
     *
     * \throws arc::ex::StateError If the FileHandle is not open.
     */
    virtual arc::int64 tell() const = 0;

    /*!
     * \brief Sets the file position indicator to the given byte index.
     *
     * \throws arc::ex::StateError If the FileHandle is not open.
     * \throws arc::ex::IndexOutOfBoundsError If the given byte index is
     *                                          greater than the number of bytes
     *                                          in the file or is less than 0.
     */
    virtual void seek(arc::int64 index) = 0;

protected:

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether this FileHandle is open or not.
     */
    bool m_open;

    /*!
     * \brief The path this FileHandle is using.
     */
    arc::io::sys::Path m_path;
    /*!
     * \brief The encoding this FileHandle is using.
     */
    Encoding m_encoding;
    /*!
     * \brief The newline symbol this FileHandle is using.
     */
    Newline m_newline;

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Default super constructor.
     *
     * Creates a new unopened FileHandle.
     *
     * \param encoding The encoding that will be used for the contents of the
     *                 file.
     * \param newline The newline symbol which the FileHandle will use.
     */
    FileHandle(
            Encoding encoding = ENCODING_DETECT,
            Newline newline = NEWLINE_UNIX);

    /*!
     * \brief Path super constructor.
     *
     * Creates a new FileHandle with the given path and opens this FileHandle to
     * it.
     *
     * \note Derived must call the open member function, as this base
     *       constructor cannot and should not call a pure virtual member
     *       function.
     *
     * \param path The path to the file to use.
     * \param encoding The encoding that will be used for the contents of the
     *                 file.
     * \param newline The newline symbol which the FileHandle will use.
     */
    FileHandle(
            const arc::io::sys::Path& path,
            Encoding encoding = ENCODING_DETECT,
            Newline newline = NEWLINE_UNIX);

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief If this FileHandle's newline is currently set to
     *        arc::io::sys::FileHandle::NEWLINE_DETECT this function will
     *        reassign the newline value based on the current Operating System.
     */
    void handle_newline_detect();
};

} // namespace sys
} // namespace io
} // namespace arc

#endif
