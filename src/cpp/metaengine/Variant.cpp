#include "metaengine/Variant.hpp"

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/str/StringOperations.hpp>
#include <arcanecore/io/sys/FileReader.hpp>

#include <json/json.h>

// TODO: REMOVE ME
#include <iostream>

namespace metaengine
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

Variant::Variant(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& default_variant,
        bool load_immediately)
    :
    Document         (apply_variant(file_path, default_variant), false),
    m_base_path      (file_path),
    m_default_variant(default_variant),
    m_current_variant(default_variant)
{
    if(load_immediately)
    {
        reload();
    }
}

Variant::Variant(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String* memory,
        const arc::str::UTF8String& default_variant,
        bool load_immediately)
    :
    Document         (apply_variant(file_path, default_variant), memory, false),
    m_base_path      (file_path),
    m_default_variant(default_variant),
    m_current_variant(default_variant)
{
    if(load_immediately)
    {
        reload();
    }
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void Variant::reload()
{
    // super call
    Document::reload();

    // load the variant
    set_variant(m_current_variant);
}

void Variant::set_variant(const arc::str::UTF8String& variant)
{
    // is the same as the current variant?
    if(variant == m_current_variant && m_variant_root != nullptr)
    {
        // do nothing
        return;
    }

    m_current_variant = variant;
    // unload the current variant
    m_variant_root.reset();

    // is the default variant?
    if(m_current_variant == m_default_variant)
    {
        // go no further
        return;
    }

    // evaluate the file path for the variant
    arc::io::sys::Path variant_path(
        apply_variant(m_base_path, m_current_variant));

    // construct an optimised string to contain the file data
    arc::str::UTF8String file_data(
        arc::str::UTF8String::Opt::SKIP_VALID_CHECK);

    // attempt to read data from the file
    bool read_success = false;
    try
    {
        // open the reader
        arc::io::sys::FileReader json_file(
            variant_path,
            arc::io::sys::FileReader::ENCODING_DETECT,
            arc::io::sys::FileReader::NEWLINE_UNIX
        );
        // read
        json_file.read(file_data);
        // close
        json_file.close();
        read_success = true;
    }
    catch(const arc::ex::ArcException& exc)
    {
        // trigger a warning
        arc::str::UTF8String error_message;
        error_message << "Failed to load data for variant \""
                      << m_current_variant << "\": " << exc.get_type() << ": "
                      << exc.get_message();
        Document::s_load_reporter(variant_path, error_message);
        return;
    }

    // if file data was read: parse
    if(read_success)
    {
        try
        {
            parse(file_data, m_variant_root);
        }
        catch(const arc::ex::ParseError& exc)
        {
            // trigger a warning
            arc::str::UTF8String error_message;
            error_message << "Failed to parse data for variant \""
                          << m_current_variant << "\" with message:\n"
                          << exc.what();
            Document::s_load_reporter(variant_path, error_message);
            return;
        }
    }
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

VisitorBase* Variant::get(
        const arc::str::UTF8String& key,
        VisitorBase* visitor)
{
    // is there variant data?
    if(m_variant_root != nullptr)
    {
        // attempt to get the JSON value
        const Json::Value* data = nullptr;
        try
        {
            data = get_value(m_variant_root.get(), key);
        }
        catch(const arc::ex::KeyError& exc)
        {
            data = nullptr;
        }

        // did we get the value?
        if(data != nullptr)
        {
            // use the visitor to retrieve from the value
            bool retrieve_success = false;
            arc::str::UTF8String unused;
            try
            {
                retrieve_success =
                    visitor->retrieve(data, key, this, unused);
            }
            catch(...)
            {
                retrieve_success = false;
            }

            // if everything was successful we're done
            if(retrieve_success)
            {
                return visitor;
            }
        }
    }

    // hand off to the super implementation
    return Document::get(key, visitor);
}

//------------------------------------------------------------------------------
//                            PRIVATE STATIC FUNCTIONS
//------------------------------------------------------------------------------

arc::io::sys::Path Variant::apply_variant(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& variant)
{
    // extract the filename from the path
    arc::io::sys::Path ret(file_path);
    arc::str::UTF8String filename(ret.get_back());
    ret.remove(ret.get_length() - 1);

    // split the filename to find the suffix
    std::vector<arc::str::UTF8String> components(filename.split("."));
    assert(!components.empty());
    // no extension on the path?
    if(components.size() == 1)
    {
        components.push_back(variant);
    }
    else
    {
        components.insert(components.end() - 1, variant);
    }

    // append the filename again and return
    ret.join(arc::str::join(components, "."));
    return ret;
}

} // namespace metaengine
