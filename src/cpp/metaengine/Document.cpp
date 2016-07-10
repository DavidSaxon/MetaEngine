#include "metaengine/Document.hpp"

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/io/sys/FileReader.hpp>

#include <json/json.h>

namespace metaengine
{

//------------------------------------------------------------------------------
//                           PRIVATE STATIC ATTRIBUTES
//------------------------------------------------------------------------------

Document::fallback_reporter Document::s_load_reporter = nullptr;
Document::fallback_reporter Document::s_get_reporter = nullptr;

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

Document::Document(const arc::io::sys::Path& file_path)
    :
    m_file_path (file_path),
    m_using_path(true),
    m_memory    (nullptr),
    m_file_root (nullptr),
    m_mem_root  (nullptr)
{
    reload();
}

Document::Document(const arc::str::UTF8String* memory)
    :
    m_using_path(false),
    m_memory    (memory),
    m_file_root (nullptr),
    m_mem_root  (nullptr)
{
    reload();
}

Document::Document(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String* memory)
    :
    m_file_path (file_path),
    m_using_path(true),
    m_memory    (memory),
    m_file_root (nullptr),
    m_mem_root  (nullptr)
{
    reload();
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Document::~Document()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

void Document::set_load_fallback_reporter(fallback_reporter func)
{
    s_load_reporter = func;
}

void Document::set_get_fallback_reporter(fallback_reporter func)
{
    s_get_reporter = func;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool Document::is_using_file_path() const
{
    return m_using_path;
}

bool Document::is_using_memory() const
{
    return m_memory != nullptr;
}

bool Document::has_valid_file_data() const
{
    return m_file_root != nullptr;
}

bool Document::has_valid_memory_data() const
{
    return m_mem_root != nullptr;
}

void Document::reload()
{
    // clean up any existing data
    m_file_root.reset();
    m_mem_root.reset();

    // load file system data
    if(m_using_path)
    {
        // construct an optimised string to contain the file data
        arc::str::UTF8String file_data(
            arc::str::UTF8String::Opt::SKIP_VALID_CHECK);

        // attempt to read data from the file
        bool read_success = false;
        try
        {
            // open the reader
            arc::io::sys::FileReader json_file(
                m_file_path,
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
            // check if mem, if not rethrow, else
            if(m_memory == nullptr)
            {
                // no fallback, rethrow
                arc::str::UTF8String error_message;
                error_message << "Failed to open file: \"" << m_file_path
                              << "\" with error: " << exc.get_type() << "\n"
                              << exc.get_message();
                throw arc::ex::InvalidPathError(error_message);
            }
            else if(s_load_reporter != nullptr)
            {
                // trigger a warning, and prepare to fallback
                arc::str::UTF8String error_message;
                error_message << "Falling back to loading data from memory "
                              << "only. Failed to open file: \"" << m_file_path
                              << "\" with error: " << exc.get_type() << "\n"
                              << exc.get_message();
                s_load_reporter(m_file_path, error_message);
            }
        }

        // if file data was read: parse
        if(read_success)
        {
            try
            {
                parse(file_data, m_file_root);
            }
            catch(const arc::ex::ParseError& exc)
            {
                if(m_memory == nullptr)
                {
                    // no fallback, rethrow
                    arc::str::UTF8String error_message;
                    error_message << "Failed to parse JSON data from file: \""
                                  << m_file_path.to_native() << "\" with "
                                  << "message:\n" << exc.what();
                    throw arc::ex::ParseError(error_message);
                }
                else if(s_load_reporter != nullptr)
                {
                    // trigger a warning, and prepare to fallback
                    arc::str::UTF8String error_message;
                    error_message << "Falling back to loading data from memory "
                                  << "only. Failed to parse JSON data from "
                                  << "file: \"" << m_file_path << "\" with "
                                  << "message:\n" << exc.what();
                    s_load_reporter(m_file_path, error_message);
                }
            }
        }
    }

    // load memory data
    if(m_memory != nullptr)
    {
        try
        {
            parse(*m_memory, m_mem_root);
        }
        catch(const arc::ex::ParseError& exc)
        {
            if(!has_valid_file_data())
            {
                // there's no valid data, rethrow
                arc::str::UTF8String error_message;
                error_message << "Failed to parse JSON data from memory: <"
                              << reinterpret_cast<arc::uint64>(m_memory)
                              << "> with message:\n" << exc.what();
                throw arc::ex::ParseError(error_message);
            }
            else if(s_load_reporter != nullptr)
            {
                // trigger a warning
                arc::str::UTF8String error_message;
                error_message << "Fallback to memory is not available for "
                              << "Document using file: \"" << m_file_path
                              << "\". Memory data failed to load with error: "
                              << exc.get_type() << ": " << exc.what();
                s_load_reporter(m_file_path, error_message);
            }

        }
    }
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void Document::parse(
        const arc::str::UTF8String& json_data,
        std::unique_ptr<Json::Value>& value)
{
    // create new JSON value
    value.reset(new Json::Value());

    // parse JSON
    Json::Reader reader;
    bool parse_sucess = reader.parse(
        json_data.get_raw(),
        json_data.get_raw() + (json_data.get_byte_length() - 1),
        *value
    );

    // if parsing failed, clean up and throw
    if(!parse_sucess)
    {
        value.reset();
        throw arc::ex::ParseError(reader.getFormattedErrorMessages().c_str());
    }
}

const Json::Value* Document::get_value(
    const Json::Value* root,
    const arc::str::UTF8String& key) const
{
    // the value to be returned
    const Json::Value* value = root;
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

} // namespace metaengine
