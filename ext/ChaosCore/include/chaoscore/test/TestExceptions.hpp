/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSCORE_TEST_EXCEPTIONS_HPP_
#define CHAOSCORE_TEST_EXCEPTIONS_HPP_

#include "chaoscore/base/Exceptions.hpp"

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
 * \brief Abstract base exception that warns of an unexpected error during
 *        testing procedures.
 */
class TestError : public chaos::ex::ChaosException
{
protected:

    TestError(
        const chaos::str::UTF8String& type,
        const chaos::str::UTF8String& message)
        :
        ChaosException(type, message)
    {
    }
};

/*!
 * \brief Warns that an error has occurred during test deceleration time.
 */
class TestDeclerationError : public TestError
{
public:

    TestDeclerationError(const chaos::str::UTF8String& message)
        :
        TestError("TestDeclerationError", message)
    {
    }
};

/*!
 * \brief Warns that an error has occurred during test runtime.
 */
class TestRuntimeError : public TestError
{
public:

    TestRuntimeError(const chaos::str::UTF8String& message)
        :
        TestError("TestRuntimeError", message)
    {
    }
};

/*!
 * \brief Warns that an invalid test path has been provided to run.
 */
class InvalidPathError : public TestError
{
public:

    InvalidPathError(const chaos::str::UTF8String& message)
        :
        TestError("InvalidPathError", message)
    {
    }
};

} // namespace ex
} // namespace test
} // namespace chaos

#endif
