#include "metaengine/visitors/Path.hpp"

#include <algorithm>

#include <json/json.h>

#include "metaengine/visitors/String.hpp"

namespace metaengine
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

PathV::PathV(metaengine::Document* external_document)
    :
    metaengine::Visitor<arc::io::sys::Path>(),
    m_external                             (external_document),
    m_is_recursive                         (false)
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

PathV& PathV::instance(metaengine::Document* external_document)
{
    static PathV v;
    v.set_external_document(external_document);
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

    // is the data a list
    if(!data->isArray())
    {
        Json::FastWriter j_writer;
        error_message << "\"" << j_writer.write(*data) << "\" cannot be "
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
            Json::FastWriter j_writer;
            error_message << "Path element \"" << j_writer.write(*child)
                          << "\" cannot be converted to UTF-8 string type.";
            return false;
        }
        // get as string
        arc::str::UTF8String element(child->asCString());

        // perform local path expansion?
        if(element.starts_with("@{") && element.ends_with("}"))
        {
            // get the key to reference
            arc::str::UTF8String ref_key(
                element.substring(2, element.get_length() - 3));

            // ensure this does not reference itself
            if(ref_key == key)
            {
                error_message << "Failed to expand reference \"" << element
                              << "\" since it references itself.";
                return false;
            }
            // ensure this does reference a key we've already visited
            if(std::find(m_visited_refs.begin(), m_visited_refs.end(), ref_key)
                  != m_visited_refs.end())
            {
                error_message << "Failed to expand reference \"" << element
                              << "\" since it is a cyclic reference.";
                return false;
            }

            // attempt to expand to path type
            try
            {
                // create a new visitor, with the history of the references
                // visited so far
                PathV sub_visitor(m_external);
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
                    error_message << "Failed to expand reference \""
                                  << element << "\" no valid path or string "
                                  << "exists in the document with that key.";
                    return false;
                }
            }
        }
        // perform external path expansion
        else if(element.starts_with("#{") && element.ends_with("}"))
        {
            // ensure there i actually a document to resolve external references
            // from.
            if(m_external == nullptr)
            {
                // set error and exit
                error_message << "Failed to expand external reference \""
                              << element << "\" since no external document was "
                              << "supplied.";
                return false;
            }

            // get the key to reference
            arc::str::UTF8String ref_key(
                element.substring(2, element.get_length() - 3));

            // attempt to expand to path type
            try
            {
                // create a new visitor, but don't consider it to be recursive
                // since it's a new document, which cannot resolve external keys
                PathV sub_visitor;
                sub_visitor.m_is_recursive = false;

                temp += *m_external->get(ref_key, sub_visitor);
            }
            catch(...)
            {
                // attempt to expand to string type
                try
                {
                    temp << *m_external->get(ref_key, UTF8StringV::instance());
                }
                catch(...)
                {
                    // set error and exit
                    error_message << "Failed to expand external reference \""
                                  << element << "\" no valid path or string "
                                  << "exists in the document with that key.";
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

metaengine::Document* PathV::get_external_document() const
{
    return m_external;
}

void PathV::set_external_document(metaengine::Document* external_document)
{
    m_external = external_document;
}

//------------------------------------------------------------------------------
//                              PATH VECTOR VISITOR
//------------------------------------------------------------------------------

PathVectorV& PathVectorV::instance()
{
    static PathVectorV v;
    return v;
}

bool PathVectorV::retrieve(
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
    std::vector<arc::io::sys::Path> temp;

    // iterate over the values
    Json::Value::const_iterator child;
    for(child = data->begin(); child != data->end(); ++child)
    {
        // attempt to get a path using the PathV visitor
        if(!PathV::instance().retrieve(&*child, key, requester, error_message))
        {
            return false;
        }
        temp.push_back(*PathV::instance());
    }

    // no errors, use the temp value
    m_value = temp;
    return true;
}

} // namespace metaengine
