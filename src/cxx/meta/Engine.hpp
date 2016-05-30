/*!
 * \file
 * \brief TODO:
 * \author David Saxon
 */
#ifndef META_ENGINE_HPP_
#define META_ENGINE_HPP_

#include <chaoscore/io/sys/Path.hpp>

namespace meta
{

bool load_from_disk(const chaos::io::sys::Path& meta_dir);

bool load_from_memory(const chaos::str::UTF8String* const meta_ptr);

bool reload();

template<typename ValueType>
ValueType get(const chaos::str::UTF8String& key);

} // namespace meta

#endif
