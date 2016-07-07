#include "metaengine/AbstractDocument.hpp"

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/str/StringOperations.hpp>

#include <json/json.h>

namespace metaengine
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

AbstractDocument::AbstractDocument()
    :
    m_root(nullptr)
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const Json::Value* AbstractDocument::get_value(
        const arc::str::UTF8String& key)
{
    // the value to be returned
    const Json::Value* value = m_root.get();
    // split the hierarchy of the key
    std::vector<arc::str::UTF8String> key_elements(key.split("."));
    arc::str::UTF8String key_so_far;
    ARC_CONST_FOR_EACH(it, key_elements)
    {
        key_so_far += *it;
        // get the value associated with this key in the hierarchy
        value = &(*value)[it->get_raw()];
        // did we get back a valid value?
        if(value->isNull())
        {
            arc::str::UTF8String error_message;
            error_message << "No value exists with the key: " << key_so_far;
            throw arc::ex::KeyError(error_message);
        }
        key_so_far += ".";
    }

    return value;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void AbstractDocument::init(const arc::str::UTF8String& json_data)
{
    // clear any current values
    m_root.reset(new Json::Value());

    // parse the provided JSON data
    Json::Reader reader;
    bool parse_sucess = reader.parse(
        json_data.get_raw(),
        json_data.get_raw() + (json_data.get_byte_length() - 1),
        *m_root
    );

    // if parsing failed then clean up and throw
    if(!parse_sucess)
    {
        m_root.reset();
        throw arc::ex::ParseError(reader.getFormattedErrorMessages().c_str());
    }
}

} // namespace metaengine
