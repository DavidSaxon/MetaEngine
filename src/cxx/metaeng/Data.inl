/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_DATA_INL_
#define METAENGINE_DATA_INL_

#include "metaeng/Data.hpp"

#include <chaoscore/base/Exceptions.hpp>
#include <chaoscore/io/sys/Path.hpp>

namespace metaeng
{

// hide from doxygen
#ifndef IN_DOXYGEN

//------------------------------------------------------------------------------
//                              GET IMPLEMENTATIONS
//------------------------------------------------------------------------------

template<typename ValueType>
inline ValueType& Data::get(
        const chaos::str::UTF8String& key,
        ValueType& value) const
{
    // get the JSON value
    const Json::Value* j_value = resolve_key(key);

    // check if the conversion is possible
    if(!is_type<ValueType>(j_value))
    {
        chaos::str::UTF8String error_message;
        error_message << "Unable to convert value for key: \"" << key << "\" "
                      << "to a value of type: <"
                      << chaos::introspect::get_typename<ValueType>() << ">";
        throw chaos::ex::TypeError(error_message);
    }

    // perform the conversion and return
    as_type<ValueType>(j_value, value);
    return value;
}

template<typename ValueType>
std::vector<ValueType>& Data::get(
        const chaos::str::UTF8String& key,
        std::vector<ValueType>& value) const
{
    // temporary vector
    std::vector<ValueType> temp;
    // get the JSON value
    const Json::Value* j_array = resolve_key(key);
    bool can_convert = true;
    // check that we have a list first
    if(!j_array->isArray())
    {
        can_convert = false;
    }
    // check and get each value
    else
    {
        Json::Value::const_iterator j_value;
        for(j_value = j_array->begin(); j_value != j_array->end(); ++j_value)
        {
            // check if the value can be converted
            if(!is_type<ValueType>(&(*j_value)))
            {
                can_convert = false;
                break;
            }
            // perform the conversion
            ValueType element;
            as_type<ValueType>(&(*j_value), element);
            temp.push_back(element);
        }
    }

    // throw if the conversion cannot be done
    if(!can_convert)
    {
        chaos::str::UTF8String error_message;
        error_message << "Unable to convert value for key: \"" << key << "\" "
                      << "to a value of type: <std::vector<"
                      << chaos::introspect::get_typename<ValueType>() << ">>";
        throw chaos::ex::TypeError(error_message);
    }

    // copy temporary and return
    value = temp;
    return value;
}

#endif
// IN_DOXYGEN

//------------------------------------------------------------------------------
//                              PATH IMPLEMENTATION
//------------------------------------------------------------------------------

/*!
 * \brief Attempts to retrieve a chaos::io::sys::Path from internal data using
 *        the key.
 *
 * See data::get().
 *
 * Paths should be defined in the JSON file as an array of strings:
 *
 * \code
 * "path_1": ["/", "path", "to", "location"]
 * \endcode
 *
 * Retrieving this value would provide the following path:
 * ```/path/to/location```.
 *
 * Paths also support expansion of other values using the ```${key}``` syntax.
 * For example:
 *
 * \code
 * "path_2": ["${path_1}", sub", "folder"]
 * \endcode
 *
 * Would be retrieved as: ```/path/to/location/sub/folder```.
 *
 * \throws chaos::ex::KeyError If the key does not exist in the data.
 * \throws chaos::ex::TypeError If the value cannot be parsed as a
 *                              chaos::io::sys::Path.
 * \throws chaos::ex::RuntimeError If the a cyclic expansion is detected (i.e.
 *                                 path_1 references path_2 but path_2
 *                                 references path_1).
 */
template<>
inline chaos::io::sys::Path& Data::get(
        const chaos::str::UTF8String& key,
        chaos::io::sys::Path& value) const
{
    // check cache first
    std::map<chaos::str::UTF8String, chaos::io::sys::Path>::const_iterator
        cached = m_path_cache.find(key);
    if(cached != m_path_cache.end())
    {
        // assign from cache entry
        value = cached->second;
    }
    else
    {
        // attempt to read the value as a string list
        std::vector<chaos::str::UTF8String> path_elements;
        // catch any errors and rethrow
        try
        {
            get(key, path_elements);
        }
        catch(chaos::ex::TypeError)
        {
            chaos::str::UTF8String error_message;
            error_message << "Unable to convert value for key: \"" << key
                          << "\" to a value of type: <chaos::io::sys::Path>. A "
                          << "list of strings was expected but not found.";
            throw chaos::ex::TypeError(error_message);
        }

        // expand the list that was retrieved
        std::vector<chaos::str::UTF8String> traversed_keys;
        traversed_keys.push_back(key);
        path_expansion(path_elements, traversed_keys);

        // cache the path
        m_path_cache[key] = chaos::io::sys::Path(path_elements);

        // assign from new cache entry
        value = m_path_cache[key];
    }

    return value;
}

// hide from doxygen
#ifndef IN_DOXYGEN

//------------------------------------------------------------------------------
//                                      BOOL
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<bool>(const Json::Value* value) const
{
    return value->isBool();
}

template<>
inline void Data::as_type<bool>(const Json::Value* value, bool& ret) const
{
    ret = value->asBool();
}

//------------------------------------------------------------------------------
//                                      INT8
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<chaos::int8>(const Json::Value* value) const
{
    return value->isInt();
}

template<>
inline void Data::as_type<chaos::int8>(
        const Json::Value* value,
        chaos::int8& ret) const
{
    ret = static_cast<chaos::int8>(value->asInt());
}

//------------------------------------------------------------------------------
//                                     UINT8
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<chaos::uint8>(const Json::Value* value) const
{
    return value->isUInt();
}

template<>
inline void Data::as_type<chaos::uint8>(
        const Json::Value* value,
        chaos::uint8& ret) const
{
    ret = static_cast<chaos::uint8>(value->asUInt());
}

//------------------------------------------------------------------------------
//                                     INT16
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<chaos::int16>(const Json::Value* value) const
{
    return value->isInt();
}

template<>
inline void Data::as_type<chaos::int16>(
        const Json::Value* value,
        chaos::int16& ret) const
{
    ret = static_cast<chaos::int16>(value->asInt());
}

//------------------------------------------------------------------------------
//                                     UINT16
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<chaos::uint16>(const Json::Value* value) const
{
    return value->isUInt();
}

template<>
inline void Data::as_type<chaos::uint16>(
        const Json::Value* value,
        chaos::uint16& ret) const
{
    ret = static_cast<chaos::uint16>(value->asUInt());
}

//------------------------------------------------------------------------------
//                                     INT32
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<chaos::int32>(const Json::Value* value) const
{
    return value->isInt();
}

template<>
inline void Data::as_type<chaos::int32>(
        const Json::Value* value,
        chaos::int32& ret) const
{
    ret = static_cast<chaos::int32>(value->asInt());
}

//------------------------------------------------------------------------------
//                                     UINT32
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<chaos::uint32>(const Json::Value* value) const
{
    return value->isUInt();
}

template<>
inline void Data::as_type<chaos::uint32>(
        const Json::Value* value,
        chaos::uint32& ret) const
{
    ret = static_cast<chaos::uint32>(value->asUInt());
}

//------------------------------------------------------------------------------
//                                     INT64
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<chaos::int64>(const Json::Value* value) const
{
    return value->isInt();
}

template<>
inline void Data::as_type<chaos::int64>(
        const Json::Value* value,
        chaos::int64& ret) const
{
    ret = static_cast<chaos::int64>(value->asInt64());
}

//------------------------------------------------------------------------------
//                                     UINT64
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<chaos::uint64>(const Json::Value* value) const
{
    return value->isUInt();
}

template<>
inline void Data::as_type<chaos::uint64>(
        const Json::Value* value,
        chaos::uint64& ret) const
{
    ret = static_cast<chaos::uint64>(value->asUInt64());
}

//------------------------------------------------------------------------------
//                                     FLOAT
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<float>(const Json::Value* value) const
{
    return value->isDouble();
}

template<>
inline void Data::as_type<float>(
        const Json::Value* value,
        float& ret) const
{
    ret = value->asFloat();
}

//------------------------------------------------------------------------------
//                                     DOUBLE
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<double>(const Json::Value* value) const
{
    return value->isDouble();
}

template<>
inline void Data::as_type<double>(
        const Json::Value* value,
        double& ret) const
{
    ret = value->asDouble();
}

//------------------------------------------------------------------------------
//                                   UTF8STRING
//------------------------------------------------------------------------------

template <>
inline bool Data::is_type<chaos::str::UTF8String>(
        const Json::Value* value) const
{
    return value->isString();
}

template<>
inline void Data::as_type<chaos::str::UTF8String>(
        const Json::Value* value,
        chaos::str::UTF8String& ret) const
{
    ret.assign(value->asCString());
}

#endif
// IN_DOXYGEN

} // namespace meta

#endif
