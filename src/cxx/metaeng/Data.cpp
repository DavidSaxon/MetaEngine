#include <algorithm>
#include <iostream>

#include <chaoscore/base/Exceptions.hpp>
#include <chaoscore/io/sys/FileReader.hpp>

#define METAENG_FROM_SOURCE
#include "metaeng/Data.hpp"
#undef METAENG_FROM_SOURCE

namespace metaeng
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

bool warn_on_fallback = false;

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

Data::Data(
        const chaos::io::sys::Path& path,
        const chaos::str::UTF8String& schema)
    :
    m_using_path(true),
    m_path      (path),
    m_mem       (nullptr),
    m_schema    (schema),
    m_root      (nullptr)
{
    reload();
}

Data::Data(
        const chaos::str::UTF8String* const mem,
        const chaos::str::UTF8String& schema)
    :
    m_using_path(false),
    m_mem       (mem),
    m_schema    (schema),
    m_root      (nullptr)
{
    reload();
}

Data::Data(
        const chaos::io::sys::Path& path,
        const chaos::str::UTF8String* const mem,
        const chaos::str::UTF8String& schema)
    :
    m_using_path(true),
    m_path      (path),
    m_mem       (mem),
    m_schema    (schema),
    m_root      (nullptr)
{
    reload();
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void Data::reload()
{
    // clean up any existing data
    m_root.reset();
    m_path_cache.clear();

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
        catch(...)
        {
            // no fallback plan, just re-throw
            if(m_mem == nullptr)
            {
                throw;
            }
            // print a warning?
            else if(warn_on_fallback)
            {
                std::cerr << "MetaEngine: Failed to open and read from file: \""
                          << m_path.to_native() << "\" Failing back to reading "
                          << "data from memory." << std::endl;
            }
        }

        // parse
        if(read_success)
        {
            parse_str(file_data, m_mem == nullptr);
        }
    }

    // if there is no data yet, attempt to fall back to memory
    if(m_root == nullptr && m_mem != nullptr)
    {
        parse_str(*m_mem, true);
    }
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void Data::parse_str(const chaos::str::UTF8String& str, bool throw_on_failure)
{
    // clear the current value
    m_root.reset(new Json::Value());

    // has a schema been provided?
    std::unique_ptr<Json::Value> schema_root(new Json::Value());
    if(!m_schema.is_empty())
    {
        // parse the schema first
        Json::Reader reader;
        bool parse_sucess = reader.parse(
            m_schema.get_raw(),
            m_schema.get_raw() + (m_schema.get_byte_length() - 1),
            *schema_root
        );
        // failed to parse schema?
        if(!parse_sucess)
        {
            chaos::str::UTF8String error_message;
            error_message << "Failed to parse JSON from schema data: \""
                          << m_schema << "\" with message:\n"
                          << reader.getFormattedErrorMessages();
            throw chaos::ex::ParseError(error_message);
        }
    }

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
            // print a warning
            std::cerr << "MetaEngine: Failed to parse JSON from file: \""
                      << m_path.to_native() << "\" with message:\n"
                      << reader.getFormattedErrorMessages() << "\nFalling "
                      << "back to reading data from memory." << std::endl;
        }
    }

    // check against the schema
    if(schema_root != nullptr)
    {
        try
        {
            check_schema(schema_root.get(), m_root.get(), "");
        }
        catch(const chaos::ex::ValidationError& exc)
        {
            // clean up
            m_root.reset();
            // re-throw
            if(throw_on_failure)
            {
                throw exc;
            }
            // fallback
            else if(warn_on_fallback)
            {
                // print warning
                std::cerr << "MetaEngine: Failed to valid JSON data from "
                          << "file: \"" << m_path.to_native() << "\" with "
                          << "error: \"" << exc.what() << "\"\nFalling back to "
                          << "reading data from memory." << std::endl;
            }
        }
    }
}

void Data::check_schema(
        const Json::Value* schema_root,
        const Json::Value* data_root,
        const chaos::str::UTF8String& parent_key)
{
    // get the names of the members from both the schema and the data
    std::vector<std::string> schema_members(schema_root->getMemberNames());
    std::vector<std::string> data_members(data_root->getMemberNames());
    // check that every member in the schema is in the data
    CHAOS_CONST_FOR_EACH(schema_member, schema_members)
    {
        chaos::str::UTF8String current_key(parent_key);
        if(!current_key.is_empty())
        {
            current_key += ".";
        }
        current_key += schema_member->c_str();

        std::vector<std::string>::iterator member_match =
            std::find(data_members.begin(), data_members.end(), *schema_member);
        if(member_match == data_members.end())
        {
            chaos::str::UTF8String error_message;
            error_message << "Failed to validate against schema, the key \""
                          << current_key << "\" is not present in the "
                          << "data.";
            throw chaos::ex::ValidationError(error_message);
        }

        // get the schema and data values
        const Json::Value schema_value(
                schema_root->get(*schema_member, Json::Value()));
        const Json::Value data_value(
                data_root->get(*schema_member, Json::Value()));

        // check whether the type matches
        bool type_mismatch = false;
        chaos::str::UTF8String expected_type;
        // null
        if(schema_value.isNull())
        {
            // no need to check the schema
            continue;
        }
        // hierarchy
        else if(schema_value.isObject())
        {
            if(!data_value.isObject())
            {
                type_mismatch = true;
                expected_type = "hierarchy";
            }
            else
            {
                // types match, but we need to traverse the hierarchy further
                check_schema(&schema_value, &data_value, current_key);
            }
        }
        // bool
        else if(schema_value.isBool() && !data_value.isBool())
        {
            type_mismatch = true;
            expected_type = "bool";
        }
        // int
        else if(schema_value.isIntegral() && !data_value.isIntegral())
        {
            type_mismatch = true;
            expected_type = "int";
        }
        // float
        else if(schema_value.isDouble() && !data_value.isDouble())
        {
            type_mismatch = true;
            expected_type = "float";
        }
        // string
        else if(schema_value.isString() && !data_value.isString())
        {
            type_mismatch = true;
            expected_type = "string";
        }
        // array
        else if(schema_value.isArray())
        {
            // check that both are arrays
            if(!data_value.isArray())
            {
                type_mismatch = true;
                expected_type = "array";
            }
            // check the arrays have the same size
            if(schema_value.size() != data_value.size())
            {
                chaos::str::UTF8String error_message;
                error_message << "Failed to validate against schema, the value "
                              << "for the key \"" << current_key << "\" does "
                              << "not match the expected size in the schema: "
                              << schema_value.size();
                throw chaos::ex::ValidationError(error_message);
            }
        }

        // was there a type mismatch
        if(type_mismatch)
        {
            chaos::str::UTF8String error_message;
            error_message << "Failed to validate against schema, the value for "
                          << "the key \"" << current_key << "\" does not "
                          << "match the expected type in the schema: "
                          << expected_type;
            throw chaos::ex::ValidationError(error_message);
        }
    }
}

void Data::path_expansion(
        std::vector<chaos::str::UTF8String>& elements,
        std::vector<chaos::str::UTF8String> traversed_keys) const
{
    // copy elements and clear
    std::vector<chaos::str::UTF8String> copy(elements);
    elements.clear();

    // iterate over the provided elements
    CHAOS_CONST_FOR_EACH(element, copy)
    {
        // does the element start with and end with the expansion syntax?
        if(!element->starts_with("${") || !element->ends_with("}"))
        {
            // no expansion, just add an continue
            elements.push_back(*element);
            continue;
        }

        // strip the expansion syntax
        chaos::str::UTF8String key(
            element->substring(2, element->get_length() - 3));

        // check that the key hasn't already been traversed
        if(std::find(traversed_keys.begin(), traversed_keys.end(), key)
           != traversed_keys.end())
        {
            chaos::str::UTF8String error_message;
            error_message << "Detected cyclic expansion when resolve value for "
                          << "path with key: \"" << traversed_keys.front()
                          << "\".";
            throw chaos::ex::RuntimeError(error_message);
        }
        traversed_keys.push_back(key);

        // attempt to get a string list based on the key
        std::vector<chaos::str::UTF8String> expanded;
        try
        {
            get(key, expanded);
        }
        catch(chaos::ex::ChaosException)
        {
            // no expansion
            elements.push_back(*element);
            continue;
        }

        // recursively expand the path we got back
        path_expansion(expanded, traversed_keys);
        // add the new elements to the return value
        CHAOS_CONST_FOR_EACH(expanded_element, expanded)
        {
            elements.push_back(*expanded_element);
        }
    }
}

//------------------------------------------------------------------------------
//                            TEMPLATE SPECIALISATIONS
//------------------------------------------------------------------------------

//-------------------------------------PATH-------------------------------------

template<>
chaos::io::sys::Path& Data::get(
        const chaos::str::UTF8String& key,
        chaos::io::sys::Path& value) const
{
    // check cache first
    std::map<chaos::str::UTF8String, chaos::io::sys::Path>::const_iterator
        cached = m_path_cache.find(key);
    if(cached != m_path_cache.end())
    {
        // assign from cache entry
        value = cached->second;
    }
    else
    {
        // attempt to read the value as a string list
        std::vector<chaos::str::UTF8String> path_elements;
        // catch any errors and rethrow
        try
        {
            get(key, path_elements);
        }
        catch(chaos::ex::TypeError)
        {
            chaos::str::UTF8String error_message;
            error_message << "Unable to convert value for key: \"" << key
                          << "\" to a value of type: <chaos::io::sys::Path>. A "
                          << "list of strings was expected but not found.";
            throw chaos::ex::TypeError(error_message);
        }

        // expand the list that was retrieved
        std::vector<chaos::str::UTF8String> traversed_keys;
        traversed_keys.push_back(key);
        path_expansion(path_elements, traversed_keys);

        // cache the path
        m_path_cache[key] = chaos::io::sys::Path(path_elements);

        // assign from new cache entry
        value = m_path_cache[key];
    }

    return value;
}

//-------------------------------------BOOL-------------------------------------

template <>
bool Data::is_type<bool>(const Json::Value* value) const
{
    return value->isBool();
}

template<>
void Data::as_type<bool>(const Json::Value* value, bool& ret) const
{
    ret = value->asBool();
}

//-------------------------------------INT8-------------------------------------

template <>
bool Data::is_type<chaos::int8>(const Json::Value* value) const
{
    return value->isInt();
}

template<>
void Data::as_type<chaos::int8>(
        const Json::Value* value,
        chaos::int8& ret) const
{
    ret = static_cast<chaos::int8>(value->asInt());
}

//------------------------------------UINT8-------------------------------------

template <>
bool Data::is_type<chaos::uint8>(const Json::Value* value) const
{
    return value->isUInt();
}

template<>
void Data::as_type<chaos::uint8>(
        const Json::Value* value,
        chaos::uint8& ret) const
{
    ret = static_cast<chaos::uint8>(value->asUInt());
}

//------------------------------------INT16-------------------------------------

template <>
bool Data::is_type<chaos::int16>(const Json::Value* value) const
{
    return value->isInt();
}

template<>
void Data::as_type<chaos::int16>(
        const Json::Value* value,
        chaos::int16& ret) const
{
    ret = static_cast<chaos::int16>(value->asInt());
}

//------------------------------------UINT16------------------------------------

template <>
bool Data::is_type<chaos::uint16>(const Json::Value* value) const
{
    return value->isUInt();
}

template<>
void Data::as_type<chaos::uint16>(
        const Json::Value* value,
        chaos::uint16& ret) const
{
    ret = static_cast<chaos::uint16>(value->asUInt());
}

//------------------------------------INT32-------------------------------------

template <>
bool Data::is_type<chaos::int32>(const Json::Value* value) const
{
    return value->isInt();
}

template<>
void Data::as_type<chaos::int32>(
        const Json::Value* value,
        chaos::int32& ret) const
{
    ret = static_cast<chaos::int32>(value->asInt());
}

//------------------------------------UINT32------------------------------------

template <>
bool Data::is_type<chaos::uint32>(const Json::Value* value) const
{
    return value->isUInt();
}

template<>
void Data::as_type<chaos::uint32>(
        const Json::Value* value,
        chaos::uint32& ret) const
{
    ret = static_cast<chaos::uint32>(value->asUInt());
}

//------------------------------------INT64-------------------------------------

template <>
bool Data::is_type<chaos::int64>(const Json::Value* value) const
{
    return value->isInt();
}

template<>
void Data::as_type<chaos::int64>(
        const Json::Value* value,
        chaos::int64& ret) const
{
    ret = static_cast<chaos::int64>(value->asInt64());
}

//------------------------------------UINT64------------------------------------

template <>
bool Data::is_type<chaos::uint64>(const Json::Value* value) const
{
    return value->isUInt();
}

template<>
void Data::as_type<chaos::uint64>(
        const Json::Value* value,
        chaos::uint64& ret) const
{
    ret = static_cast<chaos::uint64>(value->asUInt64());
}

//------------------------------------FLOAT-------------------------------------

template <>
bool Data::is_type<float>(const Json::Value* value) const
{
    return value->isDouble();
}

template<>
void Data::as_type<float>(
        const Json::Value* value,
        float& ret) const
{
    ret = value->asFloat();
}

//------------------------------------DOUBLE------------------------------------

template <>
bool Data::is_type<double>(const Json::Value* value) const
{
    return value->isDouble();
}

template<>
void Data::as_type<double>(
        const Json::Value* value,
        double& ret) const
{
    ret = value->asDouble();
}

//----------------------------------UTF8STRING----------------------------------

template <>
bool Data::is_type<chaos::str::UTF8String>(
        const Json::Value* value) const
{
    return value->isString();
}

template<>
void Data::as_type<chaos::str::UTF8String>(
        const Json::Value* value,
        chaos::str::UTF8String& ret) const
{
    ret.assign(value->asCString());
}

} // namespace metaeng
