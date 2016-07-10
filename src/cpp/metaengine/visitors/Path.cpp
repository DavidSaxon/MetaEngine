#include "metaengine/visitors/Path.hpp"

#include <json/json.h>

#include "metaengine/visitors/String.hpp"

namespace metaengine
{

PathV& PathV::instance()
{
    static PathV v;
    return v;
}

bool PathV::retrieve(
        const Json::Value* data,
        Document* requester,
        arc::str::UTF8String& error_message)
{
    // is the path a list
    if(!data->isArray())
    {
        error_message << "Data: \"" << data->toStyledString() << "\" cannot be "
                      << "converted to array type, which is required to build "
                      << "a path.";
        return false;
    }

    // temporary value
    arc::io::sys::Path temp;

    // iterate over the elements of the list
    Json::Value::const_iterator child;
    for(child = data->begin(); child != data->end(); ++child)
    {
        // check if the data can be converted
        if(!child->isString())
        {
            error_message << "Path element data: \"" << child->toStyledString()
                          << "\" cannot be converted to UTF-8 string type.";
            return false;
        }
        // get as string
        arc::str::UTF8String element(child->asCString());

        // perform path expansion?
        if(element.starts_with("@{") && element.ends_with("}"))
        {
            // attempt to expand to path type
            try
            {
                temp += *requester->get(
                    element.substring(2, element.get_length() - 3),
                    PathV::instance()
                );
            }
            catch(...)
            {
                // attempt to expand to string type
                try
                {
                    temp << *requester->get(
                        element.substring(2, element.get_length() - 3),
                        UTF8StringV::instance()
                    );
                }
                catch(...)
                {
                    // set error and exit
                    error_message << "Failed to expand reference: \""
                                  << element << "\".";
                    return false;
                }
            }
        }
        else
        {
            // no expansion, just append
            temp << element;
        }
    }

    // assign temp, and return
    m_value = temp;
    return true;
}

} // namespace metaengine
