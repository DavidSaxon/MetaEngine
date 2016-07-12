#include "metaengine/visitors/String.hpp"

#include <json/json.h>

namespace metaengine
{

//------------------------------------------------------------------------------
//                               UTF8STRING VISITOR
//------------------------------------------------------------------------------

UTF8StringV& UTF8StringV::instance()
{
    static UTF8StringV v;
    return v;
}

bool UTF8StringV::retrieve(
        const Json::Value* data,
        const arc::str::UTF8String& key,
        Document* requester,
        arc::str::UTF8String& error_message)
{
    // check type
    if(!data->isString())
    {
        Json::FastWriter j_writer;
        error_message << "\"" << j_writer.write(*data) << "\" cannot be "
                      << "converted to UTF-8 string type.";
        return false;
    }

    m_value = arc::str::UTF8String(data->asCString());
    return true;
}

//------------------------------------------------------------------------------
//                           UTF8STRING VECTOR VISITOR
//------------------------------------------------------------------------------

UTF8StringVectorV& UTF8StringVectorV::instance()
{
    static UTF8StringVectorV v;
    return v;
}

bool UTF8StringVectorV::retrieve(
        const Json::Value* data,
        const arc::str::UTF8String& key,
        Document* requester,
        arc::str::UTF8String& error_message)
{
    // check type
    if(!data->isArray())
    {
        Json::FastWriter j_writer;
        error_message << "\"" << j_writer.write(*data) << "\" cannot be "
                      << "converted to array type.";
        return false;
    }

    // temp value
    std::vector<arc::str::UTF8String> temp;

    // iterate over the values
    Json::Value::const_iterator child;
    for(child = data->begin(); child != data->end(); ++child)
    {
        // check if the data can be converted
        if(!child->isString())
        {
            Json::FastWriter j_writer;
            error_message << "Array element \"" << j_writer.write(*child)
                          << "\" cannot be converted to UTF-8 string type.";
            return false;
        }
        // perform conversion
        temp.push_back(arc::str::UTF8String(child->asCString()));
    }

    // no errors, use the temp value
    m_value = temp;
    return true;
}

} // namespace metaengine
