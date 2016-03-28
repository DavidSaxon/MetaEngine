#include "meta/Accessor.hpp"

#include <chaoscore/io/sys/FileSystemOperations.hpp>

// TODO: REMOVE ME
#include <iostream>

namespace meta
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Accessor::Accessor(const chaos::io::sys::Path& path)
    :
    m_path        (path),
    m_is_directory(chaos::io::sys::is_directory(path))
{
    // initialise based on the type
    if (m_is_directory)
    {
        directory_init();
    }
    else
    {
        file_init();
    }
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const chaos::io::sys::Path& Accessor::get_path() const
{
    return m_path;
}

bool Accessor::is_directory() const
{
    return m_is_directory;
}

const std::vector<chaos::uni::UTF8String>& Accessor::get_components() const
{
    return m_components;
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void Accessor::directory_init()
{
    // get the files under this directory that end with .meta
    std::vector<chaos::io::sys::Path> paths(chaos::io::sys::list(m_path));
    CHAOS_FOR_EACH(it, paths)
    {
        // make sure this isn't . or ..
        if (it->get_back() == "." ||
            it->get_back() == ".."   )
        {
            continue;
        }

        // if this is a file check that it ends with .meta
        if (chaos::io::sys::is_file(m_path) && m_path.get_extension() != "meta")
        {
            continue;
        }

        m_components.push_back(it->get_back());
    }
}

void Accessor::file_init()
{
    // TODO: open config file and begin reading JSON
}

} // namespace meta
