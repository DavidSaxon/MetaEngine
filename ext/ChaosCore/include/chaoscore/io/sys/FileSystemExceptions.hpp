/*!
 * \file
 * \brief Exceptions relating to the file system.
 * \author David Saxon
 */
#ifndef CHAOSCORE_IO_SYS_FILESYSTEMEXCEPTIONS_HPP_
#define CHAOSCORE_IO_SYS_FILESYSTEMEXCEPTIONS_HPP_

#include "chaoscore/base/BaseExceptions.hpp"

namespace chaos
{
namespace io
{
namespace sys
{

/*!
 * \brief Abstract base error relating to the file system.
 *
 * All other file system exceptions inherit from this exception.
 */
class FileSystemError : public chaos::ex::ChaosException
{
protected:

    FileSystemError(
        const chaos::str::UTF8String& type,
        const chaos::str::UTF8String& message)
        :
        ChaosException(type, message)
    {
    }
};

/*!
 * \brief Warns that given path cannot be accessed and/or modified because it
 *        does not exist permission is denied.
 */
class InvalidPathError : public chaos::ex::ChaosException
{
public:

    InvalidPathError(const chaos::str::UTF8String& message)
        :
        ChaosException("InvalidPathError", message)
    {
    }
};

/*!
 * \brief Warns that creating a directory has failed.
 */
class CreateDirectoryError : public FileSystemError
{
public:

    CreateDirectoryError(const chaos::str::UTF8String& message)
        :
        FileSystemError("CreateDirectoryError", message)
    {
    }
};

/*!
 * \brief Warns that has a request has been made to create a file or directory
 *        that results in a ambiguous file system path.
 *
 * Example: a directory called 'my_name' was attempted to be created where a
 * file also called 'my_name' already exists.
 */
class AmbiguousPathError : public FileSystemError
{
public:

    AmbiguousPathError(const chaos::str::UTF8String& message)
        :
        FileSystemError("AmbiguousPathError", message)
    {
    }
};

/*!
 * \brief Warns that the end of file marker has been reached.
 */
class EOFError : public FileSystemError
{
public:

    EOFError(const chaos::str::UTF8String& message)
        :
        FileSystemError("EOFError", message)
    {
    }
};

} // namespace sys
} // namespace io
} // namespace chaos

#endif
