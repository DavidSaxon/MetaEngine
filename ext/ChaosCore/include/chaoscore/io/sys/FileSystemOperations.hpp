/*!
 * \file
 * \brief Operations relating to the file system.
 * \author David Saxon
 */
#ifndef CHAOSCORE_IO_SYS_FILESYSTEMOPERATIONS
#define CHAOSCORE_IO_SYS_FILESYSTEMOPERATIONS

#include "chaoscore/io/sys/Path.hpp"

namespace chaos
{
namespace io
{
namespace sys
{

/*!
 * \brief Checks whether the given path exists on the file system.
 *
 * This operation does not concern with the type of the path, this will return
 * true if the path is a directory, file, symbolic link, or hard link.
 *
 * \param path The path to check for existence.
 * \param resolve_links Controls whether the operation will follow symbolic
 *                      links and treat them as the path it points to or if the
 *                      path to a symbolic link should be treated as the
 *                      absolute path to the link itself (default behavior).
 *                      This argument has no effect on Windows platforms since
 *                      symbolic linking is not supported by Windows.
 * \return Returns True if the path exists, false otherwise.
 */
bool exists( const chaos::io::sys::Path& path, bool resolve_links = false );

/*!
 * \brief Returns whether the given path is a regular file.
 *
 * \note If the path does not exist this operation will return false.
 *
 * \param resolve_links Controls whether the operation will follow symbolic
 *                      links and treat them as the path it points to or if the
 *                      path to a symbolic link should be treated as the
 *                      absolute path to the link itself (default behavior).
 *                      This argument has no effect on Windows platforms since
 *                      symbolic linking is not supported by Windows.
 * \return Returns True if the given path is a file, false otherwise.
 */
bool is_file( const chaos::io::sys::Path& path, bool resolve_links = false );

/*!
 * \brief Returns whether the given path is a directory.
 *
 * \note If the path does not exist this operation will return false.
 *
 * \param resolve_links Controls whether the operation will follow symbolic
 *                      links and treat them as the path it points to (default
 *                      behavior) or if the path to a symbolic link should be
 *                      treated as the absolute path to the link itself. This
 *                      argument has no effect on Windows platforms since
 *                      symbolic linking is not supported by Windows.
 * \return Returns True if the given path is a directory, false otherwise.
 */
bool is_directory(
        const chaos::io::sys::Path& path,
        bool resolve_links = true );

/*!
 * \brief Returns whether the given path is a symbolic link.
 *
 * \note If the path does not exist this operation will return false.
 */
bool is_symbolic_link( const chaos::io::sys::Path& path );

/*!
 * \brief Lists the file system paths located under the given path.
 *
 * This operation performs a similar function to the `ls` command on Linux, or
 * the `dir` command on Windows. An empty vector will be returned if the given
 * path does not exist or is .not a directory. This function will not resolve
 * a symbolic link if one is provided as the input path.
 *
 * \param path The file path to list sub-paths for.
 * \param include_special Whether the special symbols "." and ".." while be
 *                        included with the returned paths.
 */
std::vector<chaos::io::sys::Path> list(
        const chaos::io::sys::Path& path,
        bool include_special = false);

/*!
 * \brief Lists all descendant file system paths located under the given path.
 *
 * This operation returns a vector similar to that of list(), except any sub
 * paths that are also directories are traversed and so on, so that this
 * function returns all paths that are a descendant of the given path. This
 * function will not resolve symbolics in order to avoid infinite recursion.
 */
std::vector< chaos::io::sys::Path> list_rec(
        const chaos::io::sys::Path& path,
        bool include_special = false);

/*!
 * \brief Attempts to create the directory at the given path.
 *
 * \throws chaos::ex::InvalidPathError If directory creation was attempted
 *                                          but failed.
 *
 * \return True is a new directory was created, false if the directory already
 *         exists.
 */
bool create_directory( const chaos::io::sys::Path& path );

/*!
 * \brief Deletes the given Path on the file system if it exists.
 *
 * This operation will fail if the given path is a non-empty directory, see
 * chaos::io::sys::delete_path_rec() to delete non-empty directories.
 *
 * \note This operation will delete symbolic link objects but will not follow
 *       them to delete the path they point to.
 *
 * \throws chaos::ex::InvalidPathError If the path cannot be accessed
 *                                          and/or modified to be deleted.
 */
void delete_path( const chaos::io::sys::Path& path );

/*!
 * \brief Deletes the given path on the file system and all subsequent paths
 *        under the given path.
 *
 * This effectively performs the same job as chaos::io::sys::delete_path()
 * except that non-empty directories will also be deleted by this operation.
 *
 * \note This operation will delete symbolic link objects but will not follow
 *       them to delete the path they point to.
 *
 * \throws chaos::ex::InvalidPathError If a path in the directory hierarchy
 *                                          cannot be accessed and/or modified
 *                                          to be deleted.
 */
void delete_path_rec( const chaos::io::sys::Path& path );

/*!
 * \brief Attempts to ensure all directories up to the provided path exist.
 *
 * If any directories up to the final component of the provided path do not
 * exist this operation will attempt to create them. If the given path cannot be
 * validated this will fail with an exception.
 *
 * This operation is useful to ensure a path is available for writing.
 *
 * Example usage:
 *
 * \code
 * chaos::io::sys::Path p;
 * p << "example" << "path" << "to" << "filename.txt";
 * chaos::io::sys::validate( p );
 * // this will attempt to ensure example/path/to/ exists, if this operations
 * // failed an exception would have been thrown.
 * // We can now begin writing to filename.txt knowing that it's containing
 * // directory exists.
 * std::ofstream open_file( p.to_native().get_raw() );
 *
 * \endcode
 *
 * \throws chaos::ex::InvalidPathError If one of the directories in the
 *                                          path was attempted to be created but
 *                                          failed.
 */
 void validate( const chaos::io::sys::Path& path );

} // namespace sys
} // namespace io
} // namespace chaos

#endif
