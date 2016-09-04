/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_VISITOR_HPP_
#define METAENGINE_VISITOR_HPP_

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

class Document;

/*!
 * \brief Abstract base class that defines a visitor object that can be used to
 *        retrieve values from a Document.
 *
 * This base class is untyped and should not be inherited directly from, see
 * Visitor instead.
 */
class VisitorBase
{
private:

    ARC_DISALLOW_COPY_AND_ASSIGN(VisitorBase);

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor.
     */
    VisitorBase()
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~VisitorBase()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Attempts to parse the given JSON data as this Visitor's type and
     *        update its internal value.
     *
     * This function will be called when this Visitor is passed into the
     * Document::get() function. It will be passed the JSON data that is
     * associated with the key used with the Document::get() function.
     *
     * To implement this function the JSON data should be checked to see if it
     * is convertible to this Visitor's type, if not the function should
     * return ```false``` and let the Document::get() function preform the error
     * handling. If the data is convertible to the type then it should be
     * converted and stored in this object's Visitor::m_value and then should
     * return ```true```.
     *
     * \param data The JSON value to attempt to convert to this Visitor's type.
     * \param key The key that was used to retrieve the given JSON data from the
     *             Document.
     * \param requester The Document that has called this function and provided
     *                  the JSON data.
     * \param error_message Can be used to return an error message if the
     *                      conversion was not successful.
     */
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            Document* requester,
            arc::str::UTF8String& error_message) = 0;
};

/*!
 * \brief Typed implementation of BaseVisitor.
 *
 * This object provides an internal value of the template type which should be
 * set by the retrieve function in implementations of this object.
 *
 * \tparam ReturnType The type this Visitor is retrieving and returning from
 *                    Documents.
 */
template <typename ReturnType>
class Visitor : public VisitorBase
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    Visitor()
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Visitor()
    {
    }

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the value that this Visitor holds.
     */
    const ReturnType& operator*() const
    {
        return m_value;
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the value that this Visitor holds.
     */
    const ReturnType& get_value() const
    {
        return m_value;
    }

protected:

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The internal value of this Visitor. It will be set when the
     *        retrieve function completes successfully.
     */
    ReturnType m_value;
};

} // namespace metaengine

#endif
