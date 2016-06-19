/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_DATA_HPP_
#define METAENGINE_DATA_HPP_

#include <chaoscore/base/Exceptions.hpp>
#include <chaoscore/base/introspect/IntrospectOperations.hpp>
#include <chaoscore/io/sys/Path.hpp>

#include <json/json.h>

namespace metaeng
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

/*!
 * \brief Determines whether a warning should be printed to ```std::cerr``` if a
 *        metaeng::Data object fails to load data from a file path and instead
 *        fallbacks to loading from memory.
 *
 * \note This value can be set at compile time by defining
 *       ```METAENG_WARN_ON_FALLBACK```.
 */
extern bool warn_on_fallback;

/*!
 * \brief Object that can be used to load and retrieve MetaEngine data.
 */
class Data
{
private:

    CHAOS_DISALLOW_COPY_AND_ASSIGN(Data);

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief File path constructor.
     *
     * Constructs a new MetaEngine Data object by loading JSON from the given
     * file path.
     *
     * \param path File path to a JSON file to load data from.
     * \param schema JSON data the denotes the layout the input data is expected
     *               to match. If an empty string is provided, schema validation
     *               is not preformed.
     *
     * \throws chaos::ex::InvalidPathError If the path cannot be accessed.
     * \throws chaos::ex::ParseError If the file contents or the schema cannot
     *                               be parsed as JSON.
     * TODO: throw validation error
     */
    Data(
            const chaos::io::sys::Path& path,
            const chaos::str::UTF8String& schema = chaos::str::UTF8String());

    /*!
     * \brief Memory constructor.
     *
     * Constructs a new MetaEngine Data object by reading JSON from the given
     * memory address.
     *
     * \param mem Pointer to a chaos::str::UTF8String that contains the JSON to
     *            read from.
     * \param schema JSON data the denotes the layout the input data is expected
     *               to match. If an empty string is provided, schema validation
     *               is not preformed.
     *
     * \throws chaos::ex::ParseError If the memory or the schema cannot be
     *                               parsed as JSON.
     * TODO: throw validation error
     */
    Data(
            const chaos::str::UTF8String* const mem,
            const chaos::str::UTF8String& schema = chaos::str::UTF8String());

    /*!
     * \brief Fallback constructor.
     *
     * Constructs a new MetaEngine Data object by first loading JSON from the
     * given file path. If loading from the file path files the JSON will be
     * read from the memory address instead.
     *
     * \note If metaeng::warn_on_fallback is ```true``` then a warning will be
     *       printed to ```std::cerr``` if loading from the file path fails.
     *
     * \param path File path to a JSON file to load data from.
     * \param mem Pointer to a chaos::str::UTF8String that contains JSON to read
     *            from if reading from the file fails.
     * \param schema JSON data the denotes the layout the input data is expected
     *               to match. If an empty string is provided, schema validation
     *               is not preformed.
     *
     * \throws chaos::ex::ParseError If the file cannot be loaded, and the
     *                               memory or the schema cannot be parsed as
     *                               JSON.
     * TODO: throw validation error
     */
    Data(
            const chaos::io::sys::Path& path,
            const chaos::str::UTF8String* const mem,
            const chaos::str::UTF8String& schema = chaos::str::UTF8String());

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Causes the internal data of this object to be reloaded from it's
     *        source.
     *
     * The data will be reloaded using the same source that this object was
     * constructed with. i.e. if the file constructor was used the data will be
     * reloaded from the file, if the memory constructor was used the data will
     * be reread from the chaos::str::UTF8String pointer, and if the fallback
     * constructor was used, the data will be reloaded from the file and if that
     * fails then reread from the pointer
     *
     * \throws chaos::ex::InvalidPathError If the path cannot be accessed.
     * \throws chaos::ex::ParseError If the file and/or memory contents cannot
     *                               be parsed as JSON.
     */
    void reload();

    /*!
     * \brief Attempts to retrieve a typed value from the internal data using
     *        the given key.
     *
     * The data key maps directly to the keys used in the JSON data this object
     * was loaded from. Sub keys are accessed using the ```.``` character, e.g.
     *
     * \code
     * "key_1.key_2"
     * \endcode
     *
     * \tparam ValueType The type of the value to retrieve from the data.
     *
     * \param key The key of the value to retrieve from the data.
     * \param value Returns the value from the data.
     *
     * \throws chaos::ex::KeyError If the key does not exist in the data.
     * \throws chaos::ex::TypeError If the value cannot be parsed as the
     *                              ValueType.
     */
    template<typename ValueType>
    ValueType& get(
            const chaos::str::UTF8String& key,
            ValueType& value) const
    {
        // get the JSON value
        const Json::Value* j_value = resolve_key(key);

        // check if the conversion is possible
        if(!is_type<ValueType>(j_value))
        {
            chaos::str::UTF8String error_message;
            error_message << "Unable to convert value for key: \"" << key
                          << "\" to a value of type: <"
                          << chaos::introspect::get_typename<ValueType>()
                          << ">";
            throw chaos::ex::TypeError(error_message);
        }

        // perform the conversion and return
        as_type<ValueType>(j_value, value);
        return value;
    }

    /*!
     * \brief Attempts to retrieve an array value from the internal data as a
     *        typed ```std::vector```.
     *
     * See Data::get().
     */
    template<typename ValueType>
    std::vector<ValueType>& get(
            const chaos::str::UTF8String& key,
            std::vector<ValueType>& value) const
    {
        // temporary vector
        std::vector<ValueType> temp;
        // get the JSON value
        const Json::Value* j_array = resolve_key(key);
        bool can_convert = true;
        // check that we have a list first
        if(!j_array->isArray())
        {
            can_convert = false;
        }
        // check and get each value
        else
        {
            Json::Value::const_iterator j_value;
            for(
                j_value = j_array->begin();
                j_value != j_array->end();
                ++j_value)
            {
                // check if the value can be converted
                if(!is_type<ValueType>(&(*j_value)))
                {
                    can_convert = false;
                    break;
                }
                // perform the conversion
                ValueType element;
                as_type<ValueType>(&(*j_value), element);
                temp.push_back(element);
            }
        }

        // throw if the conversion cannot be done
        if(!can_convert)
        {
            chaos::str::UTF8String error_message;
            error_message << "Unable to convert value for key: \"" << key
                          << "\" to a value of type: <std::vector<"
                          << chaos::introspect::get_typename<ValueType>()
                          << ">>";
            throw chaos::ex::TypeError(error_message);
        }

        // copy temporary and return
        value = temp;
        return value;
    }

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns whether the given JSON value can be parsed as the template
     *        type.
     *
     * \note Template specialisations for this function and as_type() should be
     *       implemented to provided supported for custom types.
     */
    template <typename Type>
    bool is_type(const Json::Value* value) const
    {
        return false;
    }

    /*!
     * \brief Returns the given JSON value as the template type.
     *
     * This function will only be called if as_type() returns ```true```.
     *
     * \note Template specialisations for this function and is_type() should be
     *       implemented to provided supported for custom types.
     */
    template<typename Type>
    void as_type(const Json::Value* value, Type& ret) const
    {
        // do nothing by default
    }

    /*!
     * \brief Returns the Json Value associated with the given key.
     */
    const Json::Value* resolve_key(const chaos::str::UTF8String& key) const;

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether this meta Data is read data from a file path.
     */
    bool m_using_path;
    /*!
     * \brief The file path that data should be read from.
     */
    chaos::io::sys::Path m_path;

    /*!
     * \brief Pointer to UTF8String that data should be read from.
     */
    const chaos::str::UTF8String* m_mem;

    /*!
     * \brief Schema that is used to check whether the input data is valid or
     *        not.
     */
    chaos::str::UTF8String m_schema;

    /*!
     * \brief The root Json Value hold the internal data.
     */
    std::unique_ptr<Json::Value> m_root;

    /*!
     * \brief Caches keys to paths that have been resolved for them.
     */
    mutable std::map<chaos::str::UTF8String, chaos::io::sys::Path> m_path_cache;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Parses the JSON data from the given string to use as the internal
     *        data of this object.
     *
     * \param String containing JSON data to be parsed.
     * \param throw_on_failure Whether a parse error should be thrown if parsing
     *                         fails.
     */
    void parse_str(const chaos::str::UTF8String& str, bool throw_on_failure);

    /*!
     * \brief Recursively checks whether the values of the schema are within
     *        loaded JSON data.
     *
     * \param schema_root the root JSON node of the schema to check against.
     * \param data_root the root JSON node of the data to check against.
     *
     * \throws chaos::ex::ValidationError If a schema mismatch is found.
     */
    void check_schema(
            const Json::Value* schema_root,
            const Json::Value* data_root,
            const chaos::str::UTF8String& parent_key);

    /*!
     * \brief Replaces any elements of the given list that have expansion syntax
     *        with their expanded lists.
     *
     * \param elements When passed in should contain the elements of the path to
     *                 perform expansion on. After this function has completed
     *                 this will hold with the elements of the path with
     *                 expansion applied.
     * \param traversed_keys Retains a list of keys the have been recursively
     *                       resolved in order to detect and escape cyclic
     *                       loops. Initially this should be a list containing
     *                       the initial path's key.
     */
    void path_expansion(
            std::vector<chaos::str::UTF8String>& elements,
            std::vector<chaos::str::UTF8String> traversed_keys) const;
};

//------------------------------------------------------------------------------
//                            TEMPLATE SPECIALISATIONS
//------------------------------------------------------------------------------

/*!
 * \brief Attempts to retrieve a chaos::io::sys::Path from internal data using
 *        the key.
 *
 * See data::get().
 *
 * Paths should be defined in the JSON file as an array of strings:
 *
 * \code
 * "path_1": ["/", "path", "to", "location"]
 * \endcode
 *
 * Retrieving this value would provide the following path:
 * ```/path/to/location```.
 *
 * Paths also support expansion of other values using the ```${key}``` syntax.
 * For example:
 *
 * \code
 * "path_2": ["${path_1}", sub", "folder"]
 * \endcode
 *
 * Would be retrieved as: ```/path/to/location/sub/folder```.
 *
 * \throws chaos::ex::KeyError If the key does not exist in the data.
 * \throws chaos::ex::TypeError If the value cannot be parsed as a
 *                              chaos::io::sys::Path.
 * \throws chaos::ex::RuntimeError If the a cyclic expansion is detected (i.e.
 *                                 path_1 references path_2 but path_2
 *                                 references path_1).
 */
template<>
chaos::io::sys::Path& Data::get(
        const chaos::str::UTF8String& key,
        chaos::io::sys::Path& value) const;

//------------------------------------------------------------------------------

// hide from doxygen
#ifndef IN_DOXYGEN

//-------------------------------------BOOL-------------------------------------

template <>
bool Data::is_type<bool>(const Json::Value* value) const;

template<>
void Data::as_type<bool>(const Json::Value* value, bool& ret) const;

//-------------------------------------INT8-------------------------------------

template <>
bool Data::is_type<chaos::int8>(const Json::Value* value) const;

template<>
void Data::as_type<chaos::int8>(
        const Json::Value* value,
        chaos::int8& ret) const;

//------------------------------------UINT8-------------------------------------

template <>
bool Data::is_type<chaos::uint8>(const Json::Value* value) const;

template<>
void Data::as_type<chaos::uint8>(
        const Json::Value* value,
        chaos::uint8& ret) const;

//------------------------------------INT16-------------------------------------

template <>
bool Data::is_type<chaos::int16>(const Json::Value* value) const;

template<>
void Data::as_type<chaos::int16>(
        const Json::Value* value,
        chaos::int16& ret) const;

//------------------------------------UINT16------------------------------------

template <>
bool Data::is_type<chaos::uint16>(const Json::Value* value) const;

template<>
void Data::as_type<chaos::uint16>(
        const Json::Value* value,
        chaos::uint16& ret) const;

//------------------------------------INT32-------------------------------------

template <>
bool Data::is_type<chaos::int32>(const Json::Value* value) const;

template<>
void Data::as_type<chaos::int32>(
        const Json::Value* value,
        chaos::int32& ret) const;

//------------------------------------UINT32------------------------------------

template <>
bool Data::is_type<chaos::uint32>(const Json::Value* value) const;

template<>
void Data::as_type<chaos::uint32>(
        const Json::Value* value,
        chaos::uint32& ret) const;

//------------------------------------INT64-------------------------------------

template <>
bool Data::is_type<chaos::int64>(const Json::Value* value) const;

template<>
void Data::as_type<chaos::int64>(
        const Json::Value* value,
        chaos::int64& ret) const;

//------------------------------------UINT64------------------------------------

template <>
bool Data::is_type<chaos::uint64>(const Json::Value* value) const;

template<>
void Data::as_type<chaos::uint64>(
        const Json::Value* value,
        chaos::uint64& ret) const;

//------------------------------------FLOAT-------------------------------------

template <>
bool Data::is_type<float>(const Json::Value* value) const;

template<>
void Data::as_type<float>(
        const Json::Value* value,
        float& ret) const;

//------------------------------------DOUBLE------------------------------------

template <>
bool Data::is_type<double>(const Json::Value* value) const;

template<>
void Data::as_type<double>(
        const Json::Value* value,
        double& ret) const;

//----------------------------------UTF8STRING----------------------------------

template <>
bool Data::is_type<chaos::str::UTF8String>(
        const Json::Value* value) const;

template<>
void Data::as_type<chaos::str::UTF8String>(
        const Json::Value* value,
        chaos::str::UTF8String& ret) const;

#endif
// IN_DOXYGEN

//------------------------------------------------------------------------------

} // namespace metaeng

#endif
// METAENGINE_DATA_HPP_

//------------------------------------------------------------------------------
//                                POST DEFINITIONS
//------------------------------------------------------------------------------

// The following code is used to set the warn_on_fallback if
// METAENG_WARN_ON_FALLBACK is defined and this is the first time this file has
// been included from a non MetaEngine source file

// hide from doxygen
#ifndef IN_DOXYGEN

#ifndef METAENG_FROM_SOURCE
#ifndef METAENGINE_DATA_HPP_POST_
#define METAENGINE_DATA_HPP_POST_

namespace metaeng
{

struct WarnOnFallbackSetter
{
    WarnOnFallbackSetter(bool state)
    {
        warn_on_fallback = state;
    }
};

#ifdef METAENG_WARN_ON_FALLBACK
    static WarnOnFallbackSetter warn_on_fallback_setter(true);
#else
    static WarnOnFallbackSetter warn_on_fallback_setter(false);
#endif

} // namespace metaeng

#endif
// METAENGINE_DATA_HPP_POST_
#endif
// METAENG_FROM_SOURCE

#endif
// IN_DOXYGEN


