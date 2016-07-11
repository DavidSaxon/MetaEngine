/*!
 * \file
 * \brief Visitor objects for retrieving file system path types from Documents.
 * \author David Saxon
 */

#include <arcanecore/io/sys/Path.hpp>

#include "metaengine/Document.hpp"

namespace metaengine
{

/*!
 * \brief Visitor object used to retrieve an arc::io::sys::Path from a
 *        metaengine::Document.
 *
 * Paths must be expressed as an array of UTF-8 compatible strings in the
 * document. Paths may references other paths or strings in the same Document
 * using the \@{<other_key>} syntax.
 *
 * Example:
 *
 * \code
 * {
 *     "my_path": ["example", "path", "@{other_path}"],
 *     "other_path": ["another", "path"]
 * }
 * \endcode
 */
class PathV : public metaengine::Visitor<arc::io::sys::Path>
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    PathV();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static PathV& instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            Document* requester,
            arc::str::UTF8String& error_message);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether this Document is created as part of recursive path
     *        expansion
     */
    bool m_is_recursive;
    /*!
     * \brief List of the keys used as references already as part of the
     *        recursive path expansion.
     */
    std::vector<arc::str::UTF8String> m_visited_refs;
};

} // namespace metaengine
