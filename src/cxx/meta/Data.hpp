/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_DATA_HPP_
#define METAENGINE_DATA_HPP_

#include <chaoscore/base/introspect/IntrospectOperations.hpp>
#include <chaoscore/io/sys/Path.hpp>

#include <json/json.h>

namespace meta
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

/*!
 * \brief TODO
 */
extern bool warn_on_fallback;

/*!
 * \brief TODO:
 *
 * TODO:
 */
class Data
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------


    Data(const chaos::io::sys::Path& path);

    Data(const chaos::str::UTF8String* const str);

    Data(
            const chaos::io::sys::Path& path,
            const chaos::str::UTF8String* const str);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    void reload();

    /*!
     * \brief TODO:
     */
    template<typename ValueType>
    ValueType& get(const chaos::str::UTF8String& key, ValueType& value) const;

    /*!
     * \brief TODO:
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
     * \brief Returns the Json Value associated with the given key.
     */
    const Json::Value* resolve_key(const chaos::str::UTF8String& key) const;

    /*!
     * \brief TODO
     */
    template <typename Type>
    bool is_type(const Json::Value* value) const
    {
        return false;
    }

    /*!
     * \brief TODO:
     */
    template<typename Type>
    void as_type(const Json::Value* value, Type& ret) const
    {
        // do nothing by default
    }

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
    const chaos::str::UTF8String* m_str;

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

} // namespace meta

#endif
// METAENGINE_DATA_HPP_


// The following code is used to set the warn_on_fallback if
// META_WARN_ON_FALLBACK is defined and this is the first time this file has
// been included from a non MetaEngine source file

// hide from doxygen
#ifndef IN_DOXYGEN

#ifndef META_FROM_SOURCE
#ifndef METAENGINE_DATA_HPP_POST_
#define METAENGINE_DATA_HPP_POST_

namespace meta
{

struct WarnOnFallbackSetter
{
    WarnOnFallbackSetter(bool state)
    {
        warn_on_fallback = state;
    }
};

#ifdef META_WARN_ON_FALLBACK
    static WarnOnFallbackSetter warn_on_fallback_setter(true);
#else
    static WarnOnFallbackSetter warn_on_fallback_setter(false);
#endif

} // namespace meta

#endif
// METAENGINE_DATA_HPP_POST_
#endif
// META_FROM_SOURCE

#endif
// IN_DOXYGEN


#include "meta/Data.inl"
