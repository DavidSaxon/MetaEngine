/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_DATA_HPP_
#define METAENGINE_DATA_HPP_

#include <chaoscore/io/sys/Path.hpp>

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

namespace Json
{
class Value;
} // namespace Json

namespace meta
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

/*!
 * \brief TODO
 */
#ifdef META_WARN_ON_FALLBACK
    static bool warn_on_fallback = true;
#else
    static bool warn_on_fallback = false;
#endif

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

    // template<typename ValueType>
    // ValueType get() const
    // {

    // }

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
};

} // namespace meta

#endif
