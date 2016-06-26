/*!
 * \file
 * \brief Operations relating to code introspection.
 * \author David Saxon
 */
#ifndef ARCANECORE_BASE_INTROSPECTOPERATIONS_HPP_
#define ARCANECORE_BASE_INTROSPECTOPERATIONS_HPP_

#include <typeinfo>

#ifdef __GNUC__
    #include <cxxabi.h>
#endif

#include "arcanecore/base/str/UTF8String.hpp"

namespace arc
{
namespace introspect
{

/*!
 * \brief Returns a string representing the given template type.
 *
 * For example:
 *
 * \code
 * arc::introspect::get_typename<int>();
 * \endcode
 *
 * Would return "int".
 *
 * \note The return result of this function is not guaranteed to be consistent
 *       across different compilers.
 */
template<typename Type>
arc::str::UTF8String get_typename()
{
#ifdef __GNUC__

    arc::str::UTF8String ret("TYPE UNKNOWN");
    int status;
    char* demangled_name =
        abi::__cxa_demangle(typeid(Type).name(), NULL, NULL, &status);
    if(status == 0)
    {
        ret.assign(demangled_name);
        std::free(demangled_name);
    }
    return ret;

#else

    return arc::str::UTF8String(typeid(Type).name());

#endif
}

} // namespace introspect
} // namespace arc

#endif
