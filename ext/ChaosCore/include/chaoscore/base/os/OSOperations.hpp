#ifndef CHAOSCORE_BASE_OS_OSOPERATIONS_HPP_
#define CHAOSCORE_BASE_OS_OSOPERATIONS_HPP_

#include "chaoscore/base/str/UTF8String.hpp"

namespace chaos
{
namespace os
{

/*!
 * \brief Gets the last system error message.
 *
 * This should be used after platform specific call that has failed. The related
 * error message will attempt to be retrieved as an chaos::str::UTF8String.
 */
chaos::str::UTF8String get_last_system_error_message();

} // namespace os
} // namespace chaos

#endif
