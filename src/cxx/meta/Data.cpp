#include <iostream>

#include <chaoscore/base/Exceptions.hpp>
#include <chaoscore/io/sys/FileReader.hpp>

#define META_FROM_SOURCE
#include "meta/Data.hpp"
#undef META_FROM_SOURCE

namespace meta
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

bool warn_on_fallback = false;

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

Data::Data(const chaos::io::sys::Path& path)
    :
    m_using_path(true),
    m_path      (path),
    m_str       (nullptr),
    m_root      (nullptr)
{
    reload();
}

Data::Data(const chaos::str::UTF8String* const str)
    :
    m_using_path(false),
    m_str       (str),
    m_root      (nullptr)
{
    reload();
}

Data::Data(
        const chaos::io::sys::Path& path,
        const chaos::str::UTF8String* const str)
    :
    m_using_path(true),
    m_path      (path),
    m_str       (str),
    m_root      (nullptr)
{
    reload();
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void Data::reload()
{
    // clean any existing parsed Json
    m_root.reset();

    // should we attempt to load from the path?
    if(m_using_path)
    {
        // construct an optimised string to contain the file data
        chaos::str::UTF8String file_data(
            chaos::str::UTF8String::Opt::SKIP_VALID_CHECK);

        // attempt to read the data from the file
        bool read_success = false;
        try
        {
            // open the reader
            chaos::io::sys::FileReader json_file(
                m_path,
                chaos::io::sys::FileReader::ENCODING_DETECT,
                chaos::io::sys::FileReader::NEWLINE_UNIX
            );
            // read
            json_file.read(file_data);
            // close
            json_file.close();
            read_success = true;
        }
        catch(const std::exception& exc)
        {
            // no fallback plan, just re-throw
            if(m_str == nullptr)
            {
                throw exc;
            }
            // print an warning?
            else if(warn_on_fallback)
            {
                // print a warning
                std::cerr << "MetaEngine: Failed to open and read from file: \""
                          << m_path.to_native() << "\" Failing back to reading "
                          << "data from memory." << std::endl;
            }
        }

        // parse
        if(read_success)
        {
            parse_str(file_data, m_str == nullptr);
        }
    }

    // if there is no data yet, attempt to fall back to memory
    if(m_root == nullptr && m_str != nullptr)
    {
        parse_str(*m_str, true);
    }
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void Data::parse_str(const chaos::str::UTF8String& str, bool throw_on_failure)
{
    // clear the current value
    m_root.reset(new Json::Value());

    // parse the Json
    Json::Reader reader;
    bool parse_sucess = reader.parse(
        str.get_raw(),
        str.get_raw() + (str.get_byte_length() - 1),
        *m_root
    );

    // if parsing failed then clean up and either throw or fallback
    if(!parse_sucess)
    {
        std::cout << "parse fail" << std::endl;

        m_root.reset();
        // throw
        if(throw_on_failure)
        {
            chaos::str::UTF8String error_message;
            error_message << "Failed to parse JSON from file: \""
                          << m_path.to_native() << "\" with message:\n"
                          << reader.getFormattedErrorMessages();
            throw chaos::ex::ParseError(error_message);
        }
        // fallback
        else if(warn_on_fallback)
        {
            std::cout << "warning??" << std::endl;

            // print a warning
            std::cerr << "MetaEngine: Failed to parse JSON from file: \""
                      << m_path.to_native() << "\" with message:\n"
                      << reader.getFormattedErrorMessages() << "\nFalling "
                      << "back to reading data from memory." << std::endl;
        }
    }
}

const Json::Value* Data::resolve_key(const chaos::str::UTF8String& key) const
{
    // the value to be returned
    const Json::Value* value = m_root.get();
    // split the hierarchy of the key
    std::vector<chaos::str::UTF8String> key_elements(key.split("."));
    chaos::str::UTF8String key_so_far;
    CHAOS_CONST_FOR_EACH(it, key_elements)
    {
        key_so_far += *it;
        // get the value associated with this key in the hierarchy
        value = &(*value)[it->get_raw()];
        // did we get back a valid value?
        if(value->isNull())
        {
            chaos::str::UTF8String error_message;
            error_message << "No value exists with the key: " << key_so_far;
            throw chaos::ex::KeyError(error_message);
        }
        key_so_far += ".";
    }

    return value;
}

} // namespace meta
