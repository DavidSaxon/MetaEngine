/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSCORE_BASE_EXCEPTIONS_HPP_
#define CHAOSCORE_BASE_EXCEPTIONS_HPP_

#include <exception>

#include "chaoscore/base/str/UTF8String.hpp"

namespace chaos
{

/*!
 * \brief Base generic exceptions defined by ChaosCore.
 */
namespace ex
{

//------------------------------------------------------------------------------
//                                CHAOS EXCEPTION
//------------------------------------------------------------------------------

/*!
 * \brief Abstract base class that all ChaosCore Exceptions extend from.
 *
 * This class directly inherits from std::exception.
 */
class ChaosException : public std::exception
{
public:

    //--------------------------------DESTRUCTOR--------------------------------

    virtual ~ChaosException() throw()
    {
    }

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    /*!
     * \brief Returns the type of this exception as a string.
     */
    const chaos::str::UTF8String& get_type() const
    {
        return m_type;
    }

    /*!
     * \return The reason for the exception.
     */
    virtual const char* what() const throw()
    {
        return m_message.get_raw();
    }

    /*!
     * \return The reason for the exception.
     */
    const chaos::str::UTF8String& get_message() const
    {
        return m_message;
    }

protected:

    //-------------------------------CONSTRUCTOR--------------------------------

    /*!
     * \brief Super constructor for objects derived from ChaosException.
     *
     * \param message A message decribing the reason for the exception.
     */
    ChaosException(
            const chaos::str::UTF8String& type,
            const chaos::str::UTF8String& message)
        :
        m_type   (type),
        m_message(message)
    {
    }

private:

    //----------------------------PRIVATE ATTRIBUTES----------------------------

    // The string representing the type of this exception
    const chaos::str::UTF8String m_type;
    // The message explaining the reason for the exception
    const chaos::str::UTF8String m_message;
};

//------------------------------------------------------------------------------
//                             NOT IMPLEMENTED ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an operations has been performed that has not yet been
 *        implemented.
 */
class NotImplementedError : public ChaosException
{
public:

    NotImplementedError(const chaos::str::UTF8String& message)
        :
        ChaosException("NotImplementedError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                  VALUE ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an invalid value has been supplied.
 */
class ValueError : public ChaosException
{
public:

    ValueError(const chaos::str::UTF8String& message)
        :
        ChaosException("ValueError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                  STATE ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an action has been requested that is not valid for the
 *        current state.
 */
class StateError : public ChaosException
{
public:

    StateError(const chaos::str::UTF8String& message)
        :
        ChaosException("StateError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                              ILLEGAL ACTION ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an illegal action has been performed.
 */
class IllegalActionError : public ChaosException
{
public:

    IllegalActionError(const chaos::str::UTF8String& message)
        :
        ChaosException("IllegalActionError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                           INDEX OUT OF BOUNDS ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an index has been requested outside of the allowed bounds.
 */
class IndexOutOfBoundsError : public ChaosException
{
public:

    IndexOutOfBoundsError(const chaos::str::UTF8String& message)
        :
        ChaosException("IndexOutOfBoundsError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                   KEY ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that a map key has been requested that does not exist.
 */
class KeyError : public ChaosException
{
public:

    KeyError(const chaos::str::UTF8String& message)
        :
        ChaosException("KeyError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                ARITHMETIC ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns of a failure due to bad arithmetic, e.g. zero division error.
 */
class ArithmeticError : public ChaosException
{
public:

    ArithmeticError(const chaos::str::UTF8String& message)
        :
        ChaosException("ArithmeticError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                 ENCODING ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that data was not provided in the expected encoding format.
 */
class EncodingError : public ChaosException
{
public:

    EncodingError(const chaos::str::UTF8String& message)
        :
        ChaosException("EncodingError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                             CONVERSION DATA ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that the provided data for a type conversion was bad or invalid.
 */
class ConversionDataError : public ChaosException
{
public:

    ConversionDataError(const chaos::str::UTF8String& message)
        :
        ChaosException("ConversionDataError", message)
    {
    }
};

} // namespace ex
} // namespace chaos

#endif
