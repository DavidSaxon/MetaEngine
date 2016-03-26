#include "meta/Accessor.hpp"

namespace meta
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Accessor::Accessor(const chaos::io::sys::Path& path)
    :
    m_path(path)
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const chaos::io::sys::Path& Accessor::get_path() const
{
    return m_path;
}

} // namespace meta
