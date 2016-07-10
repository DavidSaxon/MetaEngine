/*!
 * \file
 * \brief Visitor objects for retrieving file system path types from Documents.
 * \author David Saxon
 */

#include <arcanecore/io/sys/Path.hpp>

#include "metaengine/Document.hpp"

namespace metaengine
{

//------------------------------------------------------------------------------
//                                  PATH VISITOR
//------------------------------------------------------------------------------

/*!
 * \brief Visitor object used to retrieve a arc::str::UTF8String from a
 *        metaengine::Document.
 */
class PathV : public metaengine::Visitor<arc::io::sys::Path>
{
public:

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static PathV& instance();

    // override
    virtual bool retrieve(
            const Json::Value* data,
            Document* requester,
            arc::str::UTF8String& error_message);
};

} // namespace metaengine
