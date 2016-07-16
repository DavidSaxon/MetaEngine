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
 *
 * There is also some extended functionality to reference keys from an external
 * document. Where the external document must be provided to the Visitor and
 * the ```#{<other_ley>}``` syntax is used.
 *
 * Example:
 *
 * \code
 * {
 *     "my_path":  ["example", "path", "#{key_in_another_document}"]
 * }
 * \endcode
 */
class PathV : public metaengine::Visitor<arc::io::sys::Path>
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new Path Visitor.
     *
     * \param external_document Pointer to a metaengine::Document to resolve
     *                          external references from.
     */
    PathV(metaengine::Document* external_document = nullptr);

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Provides an existing static instance of this object.
     *
     * \param external_document Pointer to a metaengine::Document which the
     *                          returned Visitor will use to resolve external
     *                          references.
     */
    static PathV& instance(metaengine::Document* external_document = nullptr);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            Document* requester,
            arc::str::UTF8String& error_message);

    /*!
     * \brief Returns a pointer to the metaengine::Document which this Visitor
     *        is using to resolve external references.
     */
    metaengine::Document* get_external_document() const;

    /*!
     * \brief Sets the metaengine::Document that will be used by this Visitor to
     *        resolve external references.
     */
    void set_external_document(metaengine::Document* external_document);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Pointer to a Document that can be used to resolve external
     *        references.
     */
    metaengine::Document* m_external;

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
