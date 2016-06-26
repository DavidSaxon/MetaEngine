/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANECORE_TEST_LOGFORMATTER_HTMLLOGFORMATTER_HPP_
#define ARCANECORE_TEST_LOGFORMATTER_HTMLLOGFORMATTER_HPP_

// hide from doxygen
#ifndef IN_DOXYGEN

#include "arcanecore/test/log_formatter/AbstractTestLogFormatter.hpp"

namespace arc
{
namespace test
{
namespace log_formatter
{

/*!
 * \brief Test Log Formatter that writes logs as HTML.
 */
class HTMLTestLogFormatter : public AbstractTestLogFormatter
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new HTML Test Log Formatter.
     */
    HTMLTestLogFormatter(arc::uint16 verbosity, std::ostream* stream);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual void open_log();

    virtual void close_log(
            arc::int32 units_passed,
            arc::int32 units_failed,
            arc::int32 units_errored,
            arc::uint64 checks_passed,
            arc::uint64 checks_failed );

    virtual void open_test(
            const arc::str::UTF8String& path,
            const arc::str::UTF8String& id);

    virtual void close_test();

    virtual void report_crash(const arc::str::UTF8String& info);

    virtual void report_check_pass(
            const arc::str::UTF8String& type,
            const arc::str::UTF8String& file,
            arc::int32 line);

    virtual void report_check_fail(
            const arc::str::UTF8String& type,
            const arc::str::UTF8String& file,
            arc::int32 line,
            const arc::str::UTF8String& message);

    virtual void write_message(const arc::str::UTF8String& message);

    virtual void finialise_test_report(
            arc::uint64 checks_passed,
            arc::uint64 checks_failed);
};

} // namespace log_formatter
} // namespace test
} // namespace arc

#endif

#endif
