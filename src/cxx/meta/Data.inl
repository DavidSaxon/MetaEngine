/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_DATA_INL_
#define METAENGINE_DATA_INL_

#include "meta/Data.hpp"

namespace meta
{

//------------------------------------------------------------------------------
//                                   UTF8STRING
//------------------------------------------------------------------------------

template<>
inline chaos::str::UTF8String& Data::get(
        const chaos::str::UTF8String& key,
        chaos::str::UTF8String& value) const
{
    // get the JSON value
    const Json::Value* j_str = resolve_key(key);
    // check that we convert
    if(!j_str->isString())
    {
        chaos::str::UTF8String error_message;
        error_message << "Unable to convert value for key: \"" << key << "\" "
                      << "to a value of type: <chaos::str::UTF8String>.";
        throw chaos::ex::TypeError(error_message);
    }

    // assign and return
    value.assign(j_str->asCString());
    return value;
}

//------------------------------------------------------------------------------
//                               UTF8STRING VECTOR
//------------------------------------------------------------------------------

template<>
inline std::vector<chaos::str::UTF8String>& Data::get(
        const chaos::str::UTF8String& key,
        std::vector<chaos::str::UTF8String>& value) const
{
    // temporary vector
    std::vector<chaos::str::UTF8String> temp;
    // get the JSON value
    const Json::Value* j_array = resolve_key(key);
    // check that we can convert
    bool can_convert = true;
    // check that we have a list first
    if(!j_array->isArray())
    {
        can_convert = false;
    }
    // check each value is a string
    else
    {
        CHAOS_FOR_EACH(j_str, (*j_array))
        {
            if(!j_str->isString())
            {
                can_convert = false;
                break;
            }
            else
            {
                temp.push_back(chaos::str::UTF8String(j_str->asCString()));
            }
        }
    }
    if(!can_convert)
    {
        chaos::str::UTF8String error_message;
        error_message << "Unable to convert value for key: \"" << key << "\" "
                      << "to a value of type: "
                      << "<std::vector<chaos::str::UTF8String>>.";
        throw chaos::ex::TypeError(error_message);
    }

    // copy list for returning
    value = temp;

    return value;
}

} // namespace meta

#endif
