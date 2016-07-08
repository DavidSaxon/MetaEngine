/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANECORE_BASE_EXCEPTIONS_HPP_
#define ARCANECORE_BASE_EXCEPTIONS_HPP_

#include <exception>

#include "arcanecore/base/str/UTF8String.hpp"

namespace arc
{

/*!
 * \brief Base generic exceptions defined by ArcaneCore.
 */
namespace ex
{

//------------------------------------------------------------------------------
//                                 ARC EXCEPTION
//------------------------------------------------------------------------------

/*!
 * \brief Abstract base class that all ArcaneCore Exceptions extend from.
 *
 * This class directly inherits from std::exception.
 */
class ArcException : public std::exception
{
public:

    //--------------------------------DESTRUCTOR--------------------------------

    virtual ~ArcException() throw()
    {
    }

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    /*!
     * \brief Returns the type of this exception as a string.
     */
    const arc::str::UTF8String& get_type() const
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
    const arc::str::UTF8String& get_message() const
    {
        return m_message;
    }

protected:

    //-------------------------------CONSTRUCTOR--------------------------------

    /*!
     * \brief Super constructor for objects derived from ArcException.
     *
     * \param message A message decribing the reason for the exception.
     */
    ArcException(
            const arc::str::UTF8String& type,
            const arc::str::UTF8String& message)
        :
        std::exception(),
        m_type        (type),
        m_message     (message)
    {
    }

private:

    //----------------------------PRIVATE ATTRIBUTES----------------------------

    // The string representing the type of this exception
    const arc::str::UTF8String m_type;
    // The message explaining the reason for the exception
    const arc::str::UTF8String m_message;
};

//------------------------------------------------------------------------------
//                                 RUNTIME ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns of an unexpected error that occurred at process runtime.
 */
class RuntimeError : public ArcException
{
public:

    RuntimeError(const arc::str::UTF8String& message)
        :
        ArcException("RuntimeError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                             NOT IMPLEMENTED ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an operations has been performed that has not yet been
 *        implemented.
 */
class NotImplementedError : public ArcException
{
public:

    NotImplementedError(const arc::str::UTF8String& message)
        :
        ArcException("NotImplementedError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                   TYPE ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an invalid type has been supplied.
 */
class TypeError : public ArcException
{
public:

    TypeError(const arc::str::UTF8String& message)
        :
        ArcException("TypeError", message)
    {
    }
};


//------------------------------------------------------------------------------
//                                  VALUE ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an invalid value has been supplied.
 */
class ValueError : public ArcException
{
public:

    ValueError(const arc::str::UTF8String& message)
        :
        ArcException("ValueError", message)
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
class StateError : public ArcException
{
public:

    StateError(const arc::str::UTF8String& message)
        :
        ArcException("StateError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                              ILLEGAL ACTION ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an illegal action has been performed.
 */
class IllegalActionError : public ArcException
{
public:

    IllegalActionError(const arc::str::UTF8String& message)
        :
        ArcException("IllegalActionError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                           INDEX OUT OF BOUNDS ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an index has been requested outside of the allowed bounds.
 */
class IndexOutOfBoundsError : public ArcException
{
public:

    IndexOutOfBoundsError(const arc::str::UTF8String& message)
        :
        ArcException("IndexOutOfBoundsError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                   KEY ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that a map key has been requested that does not exist.
 */
class KeyError : public ArcException
{
public:

    KeyError(const arc::str::UTF8String& message)
        :
        ArcException("KeyError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                ARITHMETIC ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns of a failure due to bad arithmetic, e.g. zero division error.
 */
class ArithmeticError : public ArcException
{
public:

    ArithmeticError(const arc::str::UTF8String& message)
        :
        ArcException("ArithmeticError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                 ENCODING ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that data was not provided in the expected encoding format.
 */
class EncodingError : public ArcException
{
public:

    EncodingError(const arc::str::UTF8String& message)
        :
        ArcException("EncodingError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                             CONVERSION DATA ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that the provided data for a type conversion was bad or invalid.
 */
class ConversionDataError : public ArcException
{
public:

    ConversionDataError(const arc::str::UTF8String& message)
        :
        ArcException("ConversionDataError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                VALIDATION ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that a data validation check has failed.
 */
class ValidationError : public ArcException
{
public:

    ValidationError(const arc::str::UTF8String& message)
        :
        ArcException("ValidationError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                                  PARSE ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns of error while parsing data.
 */
class ParseError : public ArcException
{
public:

    ParseError(const arc::str::UTF8String& message)
        :
        ArcException("ParseError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                               INVALID PATH ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that given path cannot be accessed and/or modified because it
 *        does not exist permission is denied.
 */
class InvalidPathError : public ArcException
{
public:

    InvalidPathError(const arc::str::UTF8String& message)
        :
        ArcException("InvalidPathError", message)
    {
    }
};

//------------------------------------------------------------------------------
//                               END OF FILE ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that the end of file marker has been reached.
 */
class EOFError : public ArcException
{
public:

    EOFError(const arc::str::UTF8String& message)
        :
        ArcException("EOFError", message)
    {
    }
};

} // namespace ex
} // namespace arc

#endif
