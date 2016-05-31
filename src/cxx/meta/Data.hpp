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
    ValueType& get(const chaos::str::UTF8String& key, ValueType& value) const
    {
        // throw here, since if the type is supported it should be hitting one
        // of the template specialisations.
        chaos::str::UTF8String error_message;
        error_message << "Attempted to get meta value of unsupported type: <"
                      << chaos::introspect::get_typename<ValueType>() << ">. ";
        throw chaos::ex::TypeError(error_message);
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
     * \brief Returns the Json Value associated with the given key.
     */
    const Json::Value* resolve_key(const chaos::str::UTF8String& key) const;
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
