/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSCORE_TEST_EXCEPTIONS_HPP_
#define CHAOSCORE_TEST_EXCEPTIONS_HPP_

#include "chaoscore/base/BaseExceptions.hpp"

namespace chaos
{
namespace test
{
/*!
 * \brief Exceptions relating to ChaosCore's testing module.
 */
namespace ex
{

/*!
 * \brief Warns of an unexpected error during testing procedures.
 */
class TestError : public chaos::ex::ChaosException
{
public:

    TestError( const chaos::uni::UTF8String& message )
        :
        ChaosException( message )
    {
    }
};

/*!
 * \brief Warns that an error has occurred during test deceleration time.
 */
class TestDeclerationError : public TestError
{
public:

    TestDeclerationError( const chaos::uni::UTF8String& message )
        :
        TestError( message )
    {
    }
};

/*!
 * \brief Warns that an error has occurred during test runtime.
 */
class TestRuntimeError : public TestError
{
public:

    TestRuntimeError( const chaos::uni::UTF8String& message )
        :
        TestError( message )
    {
    }
};

/*!
 * \brief Warns that an invalid test path has been provided to run.
 */
class InvalidPathError : public TestError
{
public:

    InvalidPathError( const chaos::uni::UTF8String& message )
        :
        TestError( message )
    {
    }
};

} // namespace ex
} // namespace test
} // namespace chaos

#endif
