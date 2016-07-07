/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_ABSTRACTDOCUMENT_HPP_
#define METAENGINE_ABSTRACTDOCUMENT_HPP_

#include <memory>

#include <arcanecore/base/str/UTF8String.hpp>

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

namespace Json
{
class Value;
} // namespace Json

namespace metaengine
{

/*!
 * \brief TODO
 */
class AbstractDocument
{
private:

    ARC_DISALLOW_COPY_AND_ASSIGN(AbstractDocument);

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor for MetaEngine Documents.
     */
    AbstractDocument();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~AbstractDocument()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    virtual void reload() = 0;

    /*!
     * \brief Retrieves the raw Json::Value that that exists in this document
     *        with the given key.
     *
     * \throws arc::ex::KeyError If there does not exist a value with the given
     *                           key in this document.
     */
    const Json::Value* get_value(const arc::str::UTF8String& key);

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     *
     * TODO: throw
     */
    void init(const arc::str::UTF8String& json_data);

private:

    /*!
     * \brief The root JSON value of this Document.
     */
    std::unique_ptr<Json::Value> m_root;
};

} // namespace metaengine

#endif
