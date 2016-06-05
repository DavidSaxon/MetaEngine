/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_DATA_HPP_
#define METAENGINE_DATA_HPP_

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
     *
     * \throws chaos::ex::InvalidPathError If the path cannot be accessed.
     * \throws chaos::ex::ParseError If the file contents cannot be parsed as
     *                               JSON.
     */
    Data(const chaos::io::sys::Path& path);

    /*!
     * \brief Memory constructor.
     *
     * Constructs a new MetaEngine Data object by reading JSON from the given
     * memory address.
     *
     * \param mem Pointer to a chaos::str::UTF8String that contains the JSON to
     *            read from.
     *
     * \throws chaos::ex::ParseError If the memory cannot be parsed as JSON.
     */
    Data(const chaos::str::UTF8String* const mem);

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
     *
     * \throws chaos::ex::ParseError If the file cannot be loaded, and the
     *                               memory cannot be parsed as JSON.
     */
    Data(
            const chaos::io::sys::Path& path,
            const chaos::str::UTF8String* const mem);

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
    ValueType& get(const chaos::str::UTF8String& key, ValueType& value) const;

    /*!
     * \brief Attempts to retrieve an array value from the internal data as a
     *        typed ```std::vector```.
     *
     * See Data::get().
     */
    template<typename ValueType>
    std::vector<ValueType>& get(
            const chaos::str::UTF8String& key,
            std::vector<ValueType>& value) const;

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

} // namespace metaeng

#endif
// METAENGINE_DATA_HPP_


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
#include "metaeng/Data.inl"

#endif
// METAENG_FROM_SOURCE

#endif
// IN_DOXYGEN


