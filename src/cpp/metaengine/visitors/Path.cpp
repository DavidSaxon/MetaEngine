#include "metaengine/visitors/Path.hpp"

#include <algorithm>

#include <json/json.h>

#include "metaengine/visitors/String.hpp"

namespace metaengine
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

PathV::PathV()
    :
    metaengine::Visitor<arc::io::sys::Path>(),
    m_is_recursive(false)
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

PathV& PathV::instance()
{
    static PathV v;
    return v;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool PathV::retrieve(
        const Json::Value* data,
        const arc::str::UTF8String& key,
        Document* requester,
        arc::str::UTF8String& error_message)
{
    // if this visitor has not been recursively created ensure we have no
    // records of any visited reference keys
    if(!m_is_recursive)
    {
        m_visited_refs.clear();
    }

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
            // get the key to reference
            arc::str::UTF8String ref_key(
                element.substring(2, element.get_length() - 3));

            // ensure this does not reference itself
            if(ref_key == key)
            {
                error_message << "Failed to expand reference: \"" << element
                              << "\" since it references itself, this Data "
                              << "value.";
                return false;
            }
            // ensure this does reference a key we've already visited
            if(std::find(m_visited_refs.begin(), m_visited_refs.end(), ref_key)
                  != m_visited_refs.end())
            {
                error_message << "Failed to expand reference: \"" << element
                              << "\" since it is a a cyclic reference.";
                return false;
            }

            // attempt to expand to path type
            try
            {
                // create a new visitor, with the history of the references
                // visited so far
                PathV sub_visitor;
                sub_visitor.m_is_recursive = true;
                sub_visitor.m_visited_refs = m_visited_refs;
                sub_visitor.m_visited_refs.push_back(key);

                temp += *requester->get(ref_key, sub_visitor);
            }
            catch(...)
            {
                // attempt to expand to string type
                try
                {
                    temp << *requester->get(ref_key, UTF8StringV::instance());
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
