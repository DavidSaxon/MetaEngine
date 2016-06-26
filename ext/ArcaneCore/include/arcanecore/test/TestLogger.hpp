/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANECORE_TEST_TESTLOGGER_HPP_
#define ARCANECORE_TEST_TESTLOGGER_HPP_

// hide from doxygen
#ifndef IN_DOXYGEN

#include <iosfwd>

#include <map>
#include <vector>

#include "arcanecore/base/str/UTF8String.hpp"
#include "arcanecore/io/sys/Path.hpp"

namespace arc
{
namespace test
{

//------------------------------------------------------------------------------
//                             FORWARD DECELERATIONS
//------------------------------------------------------------------------------

namespace log_formatter
{
    class AbstractTestLogFormatter;
} // namespace log_formatter

/*!
 * \brief Interface used for managing the different loggers being used.
 */
class TestLogger
{
private:

    ARC_DISALLOW_COPY_AND_ASSIGN(TestLogger);

public:

    //--------------------------------------------------------------------------
    //                                ENUMERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief The different possible log outputs
     */
    enum OutFormat
    {
        /// Output plain text with no decoration or formatting.
        OUT_PLAIN_TEXT,
        /// Output plain text with decoration and formatting.
        OUT_PRETTY_TEXT,
        /// Output in XML format.
        OUT_XML,
        /// Output in HTML format for viewing in a web browser.
        OUT_HTML
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a test logger interface.
     *
     * By default the logger has no defined outputs.
     */
    TestLogger();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~TestLogger();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Sets the global testing id for the logger.
     */
    void set_global_id(const arc::str::UTF8String& id);

    /*!
     * \brief Sets this logger as the parent logger.
     *
     * There should be only one parent  logger. The parent logger will merge in
     * any sub logs made child processes and will also handle actions like
     * opening and closing the log.
     */
    void set_as_parent(bool state);

    /*!
     * \brief Adds a standard output for writing.
     *
     * \warning Currently writing to only one standard output is supported.
     *          Calling this function multiple times will result in an
     *          exception.
     *
     * \param verbosity The verbosity level of standard output.
     * \param format The format to write standard output in.
     */
    void add_stdout(arc::uint16 verbosity, OutFormat format);

    /*!
     * \brief Adds a file output for writing.
     *
     * \param path Full path to the file to write to. This path does not need to
     *             exist beforehand.
     * \param verbosity The verbosity level of the file output.
     * \param format The format to write to the file in.
     */
    void add_file_output(
            const arc::str::UTF8String& path,
            arc::uint16 verbosity,
            OutFormat format);

    /*!
     * \brief Opening statement of the log.
     */
    void open_log();

    /*!
     * \brief Closing statement of the log.
     */
    void close_log();

    /*!
     * \brief Opening statement of a unit test.
     *
     * \param path The Path of this test
     * \param id The unique id of this test.
     */
    void open_test(
            const arc::str::UTF8String& path,
            const arc::str::UTF8String& id);

    /*!
     * \brief Closing of a unit test.
     *
     * \param id The unique id of this test.
     */
    void close_test(const arc::str::UTF8String& id);

    /*!
     * \breif Reports a unit test crash.
     */
    void report_crash(
            const arc::str::UTF8String& id,
            const arc::str::UTF8String& info);

    /*!
     * \brief Reports a check pass.
     *
     * \param type the Type of check that passed.
     * \param file The source file that the pass occurred in.
     * \param line The line in the file that pass occurred at.
     */
    void report_check_pass(
            const arc::str::UTF8String& type,
            const arc::str::UTF8String& file,
            arc::int32 line);

    /*!
     * \brief Reports a check failure.
     *
     * \param type the Type of check that failed.
     * \param file The source file that the failure occurred in.
     * \param line The line in the file that failure occurred at.
     * \param message A message that was provided with the failure.
     */
    void report_check_fail(
            const arc::str::UTF8String& type,
            const arc::str::UTF8String& file,
            arc::int32 line,
            const arc::str::UTF8String& message);

    /*!
     * \brief Writes a message to the to test log.
     *
     * \param message The contents of the message to write.
     */
    void write_message(const arc::str::UTF8String& message);

    /*!
     * \brief Finalises reports for a unit test
     */
    void finialise_test_report();

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Global id of this logger.
     */
    arc::str::UTF8String m_global_id;
    /*!
     * \brief Whether this is the parent logger or not.
     */
    bool m_is_parent;

    /*!
     * \brief List of TestLogFormatters being used by this logger.
     */
    std::vector<log_formatter::AbstractTestLogFormatter*> m_formatters;
    /*!
     * \brief Whether the logger is using the standard output stream.
     */
    bool m_using_stdout;
    /*!
     * \brief Mapping from filenames to the streams writing to them.
     */
    std::map<arc::str::UTF8String, std::ostream*> m_file_streams;

    /*!
     * \brief Path to metadata file.
     */
    arc::io::sys::Path m_meta_path;

    /*!
     * \brief The number of unit tests that have had a 100% pass rate.
     */
    arc::uint32 m_unit_passes;
    /*!
     * \brief The number of unit tests that have not had a 100% pass rate, but
     *        still completed.
     */
    arc::uint32 m_unit_failures;
    /*!
     * \brief The number of unit tests that encountered a critical error during
     *        execution.
     */
    arc::uint32 m_unit_errors;
    /*!
     * \brief Total number of checks passed across all tests.
     */
    arc::uint64 m_global_check_pass_count;
    /*!
     * \brief Total number of checks failed across all tests.
     */
    arc::uint64 m_global_check_fail_count;

    /*!
     * \brief The number of checks passed in the current unit.
     */
    arc::uint64 m_check_pass_count;
    /*!
     * \brief The number of checks failed in the current unit.
     */
    arc::uint64 m_check_fail_count;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Internal function for creating and storing a formatter for the
     *        given stream and format.
     */
    void create_formatter(
            std::ostream* stream,
            arc::uint16 verbosity,
            OutFormat format,
            bool is_stdout = false);
};

} // namespace test
} // namespace arc

#endif

#endif
