#include <iostream>

#include <chaoscore/io/sys/FileReader.hpp>

#include <json/json.h>

#include "meta/Data.hpp"

namespace meta
{

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
        }
        catch(const std::exception& exc)
        {
            // no fallback plan, just rethrow
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

        // parse the Json
        m_root.reset(new Json::Value());
        Json::Reader reader;
        bool parse_sucess = reader.parse(
            file_data.get_raw(),
            file_data.get_raw() + (file_data.get_byte_length() - 1),
            *m_root
        );

        // if parsing failed then clean up and either throw or fallback
        if(!parse_sucess)
        {
            m_root.reset();
            // throw
            if(m_str == nullptr)
            {
                // TODO: parse error
            }
            // fallback
            else if(warn_on_fallback)
            {
                // print a warning
                std::cerr << "MetaEngine: Failed to parse JSON from file: \""
                          << m_path.to_native() << "\" Failing back to reading "
                          << "data from memory." << std::endl;
            }
        }
    }

    // if there is no data yet, attempt to fall back to memory
    // if(json_data.is_empty() && m_str != nullptr)
    // {
    //     json_data
    // }
}

} // namespace meta
