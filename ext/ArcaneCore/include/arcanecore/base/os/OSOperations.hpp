#ifndef ARCANECORE_BASE_OS_OSOPERATIONS_HPP_
#define ARCANECORE_BASE_OS_OSOPERATIONS_HPP_

#include "arcanecore/base/str/UTF8String.hpp"

namespace arc
{
namespace os
{

/*!
 * \brief Gets the last system error message.
 *
 * This should be used after platform specific call that has failed. The related
 * error message will attempt to be retrieved as an arc::str::UTF8String.
 */
arc::str::UTF8String get_last_system_error_message();

} // namespace os
} // namespace arc

#endif
