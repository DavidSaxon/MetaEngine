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

bool BoolV::retrieve(const Json::Value* value)
{
    // check type
    if(!value->isBool())
    {
        return false;
    }

    m_value = value->asBool();
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

bool BoolVectorV::retrieve(const Json::Value* value)
{
    // check type
    if(!value->isArray())
    {
        return false;
    }

    // temp value
    std::vector<bool> temp;

    // iterate over the values
    Json::Value::const_iterator child;
    for(child = value->begin(); child != value->end(); ++child)
    {
        // check if the value can be converted
        if(!child->isBool())
        {
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
