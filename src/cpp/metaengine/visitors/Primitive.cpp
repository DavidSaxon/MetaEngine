#include "metaengine/visitors/Primitive.hpp"

namespace metaengine
{

//------------------------------------------------------------------------------
//                                BOOLEAN VISITOR
//------------------------------------------------------------------------------

BoolV& BoolV::instance()
{
    static BoolV v;
    return v;
}

bool BoolV::retrieve(
        const Json::Value* data,
        Document* requester,
        arc::str::UTF8String& error_message)
{
    // check type
    if(!data->isBool())
    {
        error_message << "Data: \"" << data->toStyledString() << "\" cannot be "
                      << "converted to boolean type.";
        return false;
    }

    m_value = data->asBool();
    return true;
}

//------------------------------------------------------------------------------
//                             BOOLEAN VECTOR VISITOR
//------------------------------------------------------------------------------

BoolVectorV& BoolVectorV::instance()
{
    static BoolVectorV v;
    return v;
}

bool BoolVectorV::retrieve(
        const Json::Value* data,
        Document* requester,
        arc::str::UTF8String& error_message)
{
    // check type
    if(!data->isArray())
    {
        error_message << "Data: \"" << data->toStyledString() << "\" cannot be "
                      << "converted to array type.";
        return false;
    }

    // temp value
    std::vector<bool> temp;

    // iterate over the values
    Json::Value::const_iterator child;
    for(child = data->begin(); child != data->end(); ++child)
    {
        // check if the data can be converted
        if(!child->isBool())
        {
            error_message << "Array element data: \"" << child->toStyledString()
                          << "\" cannot be converted to boolean type.";
            return false;
        }
        // perform conversion
        temp.push_back(child->asBool());
    }

    // no errors, use the temp value
    m_value = temp;
    return true;
}

} // namespace metaengine
