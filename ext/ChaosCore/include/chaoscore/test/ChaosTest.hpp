/*!
 * \file
 * The ChaosCore testing module.
 * \author David Saxon
 */
#ifndef CHAOSCORE_TEST_CHAOSTEST_HPP_
#define CHAOSCORE_TEST_CHAOSTEST_HPP_

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include "chaoscore/base/Preproc.hpp"
#include "chaoscore/base/math/MathOperations.hpp"
#include "chaoscore/base/str/UTF8String.hpp"
#include "chaoscore/test/TestExceptions.hpp"
#include "chaoscore/test/TestLogger.hpp"

namespace chaos
{
/*!
 * \brief ChaosCore's testing module.
 */
namespace test
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief The main function that should be called to run tests.
 */
int deferred_main(int argc, char* argv[]);

/*!
 * \brief Registers a globals fixture functions.
 *
 * \param setup Function that will be called once when the test engine begins.
 * \param teardown Function that will be called once the test engine ends.
 */
void register_global_fixture(void (*setup)(), void (*teardown)());

//------------------------------------------------------------------------------
//                                    CLASSES
//------------------------------------------------------------------------------

/*!
 * \brief A Fixture is an object that can be used to set up a unit test
 *        environment.
 *
 * You should implement your own derived instances of this class to define
 * an object to use for one or more unit test's environment(s). You should never
 * have to manually instantiation these objects but their name should be
 * provided to the #CHAOS_TEST_UNIT_FIXTURE macro. If the same Fixture type is
 * used for #CHAOS_TEST_UNIT_FIXTURE calls they will be different instances of
 * the object.
 */
class Fixture
{
public:

    virtual ~Fixture()
    {
    }

    /*!
     * \brief Called right before a Unit Test using this Fixture is run.
     *
     * Use this function to set up the test environment and any needed data.
     */
    virtual void setup()
    {
    }

    /*!
     * \brief Called right after a Unit Test using this Fixture is run.
     *
     * Use this function to clean up the test environment, if needed.
     *
     * This function will be called whether the Unit Test passes or fails, but
     * will not be run if the test encounters a critical error since this means
     * the process has crashed.
     */
    virtual void teardown()
    {
    }
};

// hide from doxygen
#ifndef IN_DOXYGEN

namespace internal
{

//------------------------------------------------------------------------------
//                                    CLASSES
//------------------------------------------------------------------------------

/*!
 * \brief Structure used to wrap up a unit test. UnitTests will override this
 * class and implement the execute member function.
 */
class UnitTest
{
public:

    UnitTest( const chaos::str::UTF8String& name ) : m_name( name )
    {
    }

    virtual ~UnitTest()
    {
    }

    const chaos::str::UTF8String& get_name()
    {
        return m_name;
    }

    virtual Fixture* get_fixture() = 0;

    virtual void execute() = 0;

private:

    chaos::str::UTF8String m_name;
};

/*!
 * \brief Simple structure for storing verbosity level and format for output
 *        option.
 */
struct OutInfo
{
    chaos::uint16 verbosity;
    TestLogger::OutFormat format;

    OutInfo( chaos::uint8 v, TestLogger::OutFormat f )
        :
        verbosity( v ),
        format   ( f )
    {
    }
};

/*!
 * \brief Structure containing information about the test run configuration,
 */
struct RunInfo
{
    // the current testing id
    chaos::str::UTF8String id;
    // whether the tests should be run in a single process or not
    bool single_proc;
    // whether the test is being run as a sub-process of a parent testing
    // process
    bool sub_proc;
    // the paths to the tests to run
    std::set< chaos::str::UTF8String > paths;
    // whether the standard output stream is being used
    bool use_stdout;
    // information for stdout
    OutInfo stdout_info;
    // mapping from file path to write to, to the format to use
    std::map< chaos::str::UTF8String, OutInfo* > files;

    RunInfo()
        :
        single_proc( false ),
        sub_proc   ( false ),
        use_stdout ( true ),
        stdout_info( 3, TestLogger::OUT_PRETTY_TEXT )
    {
    }
};

/*!
 * \brief Hacky object for performing test magic.
 *
 * For oddities relating to the static global variables used this object is
 * declared entirely inline.
 */
class TestCore
{
public:

    //-------------------------------CONSTRUCTOR--------------------------------

    /*!
     * \brief This constructor should be used like a function.
     *
     * This constructor performs multiple operations: declaring test modules,
     * declaring unit tests, and running tests. Which operations are performed
     * are dependent on which parameters are provided.
     */
    TestCore(
            const chaos::str::UTF8String& path,
                  UnitTest*               unit_test,
            const chaos::str::UTF8String& file,
                  chaos::int32            line,
                  bool                    module  = false,
                  RunInfo*                run_info = NULL )
    {
        try
        {
            // run tests
            if ( run_info )
            {
                TestCore::setup( run_info );
                TestCore::run( run_info );
                TestCore::teardown( run_info );
                return;
            }

            // module declaration
            if ( module )
            {
                TestCore::declare_module( path, file, line );
                return;
            }

            // unit declaration
            TestCore::declare_unit( path, unit_test, file, line );
        }
        // invalid pass errors are handled by main
        catch ( const chaos::test::ex::InvalidPathError& e )
        {
            throw e;
        }
        catch ( chaos::test::ex::TestError& e )
        {
            // TODO: should this go to loggers?
            std::cerr << e.get_message() << std::endl;
            throw e;
        }
    }

    //-------------------------STATIC VARIABLE GETTERS--------------------------

    static TestLogger& logger()
    {
        static TestLogger l;
        return l;
    }

    static std::map< chaos::str::UTF8String, UnitTest* >& test_map()
    {
        static std::map< chaos::str::UTF8String, UnitTest* > t_m;
        return t_m;
    }

    static std::set< chaos::str::UTF8String >& base_modules()
    {
        static std::set< chaos::str::UTF8String > b_m;
        return b_m;
    }

    static std::set< chaos::str::UTF8String >& known_modules()
    {
        static std::set< chaos::str::UTF8String > k_m;
        return k_m;
    }

    static chaos::str::UTF8String& current_module()
    {
        static chaos::str::UTF8String c_m;
        return c_m;
    }

private:

    //-------------------------PRIVATE MEMBER FUNCTIONS-------------------------

    /*!
     * \brief Declares a test module.
     */
    static void declare_module(
            const chaos::str::UTF8String& path,
            const chaos::str::UTF8String& file,
                  chaos::int32            line );

    /*!
     * \brief Declares a unit test.
     */
    static void declare_unit(
            const chaos::str::UTF8String& path,
                  UnitTest*               unit_test,
            const chaos::str::UTF8String& file,
                  chaos::int32            line );

    /*!
     * \brief Sets up the state to begin running test.
     */
    static void setup( RunInfo* run_info );

    /*!
     * \brief Cleanup after tests have been run.
     */
    static void teardown( RunInfo* run_info );

    /*!
     * Runs tests defined by the run configuration information.
     */
    static void run( RunInfo* run_info );

    /*!
     * \brief runs the single given unit test with the given run configuration.
     */
    static void run_test(
            UnitTest*                     unit_test,
            const chaos::str::UTF8String& full_path,
            RunInfo*                      run_info );

    /*!
     * \brief Runs the test on this current process.
     */
    static void run_current_proc(
            UnitTest*                     unit_test,
            const chaos::str::UTF8String& full_path,
            RunInfo*                      run_info );

    /*!
     * \brief Runs the test on this current process with no log open and close.
     */
    static void run_current_proc_no_open(
            UnitTest* unit_test,
            RunInfo*  run_info );

    /*!
     * \brief Runs the current test in a new process.
     */
    static void run_new_proc(
            UnitTest*                     unit_test,
            const chaos::str::UTF8String& full_path,
            RunInfo*                      run_info );

    /*!
     * \brief Generates a new unique id for the given unit test name.
     */
    static chaos::str::UTF8String generate_id(
            const chaos::str::UTF8String& name );

    /*!
     * \brief Converts a test logger output format to a UTF8String for the
     * command line.
     */
    static chaos::str::UTF8String log_format_to_string(
            TestLogger::OutFormat format );

    /*!
     * \brief Formats and throws a TestDeclerationError.
     */
    static void throw_error(
            const chaos::str::UTF8String& message,
            const chaos::str::UTF8String& file,
                  chaos::int32            line );
};

} // namespace internal

#endif
// IN_DOXYGEN


//------------------------------------------------------------------------------
//                                     MACROS
//------------------------------------------------------------------------------

/*!
 * \brief Declares the Test Module Path for the current file.
 *
 * Files containing ChaosCore Unit Test must use this macro once and only once
 * before any Unit Test decelerations. Unit Test paths are defined by their name
 * and the module path in their respective file.
 *
 * For example, file_1.cpp has two tests named test_a and test_b and the
 * following module declaration:
 * \code
 * CHAOS_TEST_MODULE( example.file_1 )
 * \endcode
 * And file_2.cpp has one test named test_c and the following module
 * declaration:
 * \code
 * CHAOS_TEST_MODULE( example.file_2 )
 * \endcode
 *
 * The path:
 * \code
 * example.file_1.test_a
 * \endcode
 * defines the Unit Test in file_1.cpp called test_a.
 *
 * The path:
 * \code
 * example.file_1
 * \endcode
 * defines the Units Tests test_a and test_b.
 *
 * The path:
 * \code
 * example
 * \endcode
 * defines all three Unit Tests: test_a, test_b, and test_b since they all share
 * the same parent module: example.
 */
#define CHAOS_TEST_MODULE( path )                                              \
        namespace                                                              \
        {                                                                      \
        chaos::test::internal::TestCore t( #path, NULL, "", 0, true );         \
        }

/*!
 * \brief Defines a ChaosCore Unit Test with no chaos::test::Fixture.
 *
 * See #CHAOS_TEST_UNIT_FIXTURE for more information.
 */
#define CHAOS_TEST_UNIT( name ) \
        CHAOS_TEST_UNIT_FIXTURE( name, chaos::test::Fixture )

/*!
 * \brief Defines a ChaosCore Unit Test with a chaos::test::Fixture.
 *
 * Use this macro like a function definition to declare a Unit Test. Example:
 * \code
 * CHAOS_TEST_UNIT_FIXTURE( example_test, MyFixtureType )
 * {
 *     fixture->do_something();
 *
 *     CHAOS_CHECK_EQUAL( 1, 2 );
 *     // more testing code here
 *     // ...
 * }
 * \endcode
 *
 * \param name The name of this this Unit Test. The full name (module path +
 *             unit test name) for this test must be unique.
 * \param fixture_type The class name of the chaos::test::Fixture to use for
 *                     this Unit test. ChaosCore will handle instantiating this
 *                     object, and the fixture's startup and teardown functions
 *                     will be called before and after running this test
 *                     respectively. The instance of the fixture can be accessed
 *                     from within this test using the keyword:
 *                     \code
 *                     fixture
 *                     \endcode
 *                     which provides a pointer to the fixture instance.
 */
#define CHAOS_TEST_UNIT_FIXTURE( name, fixture_type )                          \
    namespace                                                                  \
    {                                                                          \
    struct name : public chaos::test::internal::UnitTest                       \
    {                                                                          \
        fixture_type* fixture;                                                 \
        name() : UnitTest( #name ), fixture( nullptr ){}                       \
        virtual ~name(){ delete fixture; }                                     \
        virtual chaos::test::Fixture* get_fixture()                            \
        {                                                                      \
            if (fixture == nullptr)                                            \
            {                                                                  \
                fixture = new fixture_type();                                  \
            }                                                                  \
            return fixture;                                                    \
        }                                                                      \
        virtual void execute();                                                \
    };                                                                         \
    static chaos::test::internal::TestCore object_##name (                     \
            #name, new name(), __FILE__, __LINE__ );                           \
    }                                                                          \
    void name::execute()

/*!
 * \brief Defines a message to be written to test logs.
 *
 * Messages are displayed in test logs with a verbosity of 3+.
 *
 * \param message The message to write to the logs. This should be a
 *                chaos::str::UTF8String or implicitly constructible as a
 *                chaos::str::UTF8String.
 */
#define CHAOS_TEST_MESSAGE( message )                                          \
    chaos::test::internal::TestCore::logger().write_message( message )

/*!
 * \brief Checks whether the given value evaluates to true.
 *
 * If a evaluates to true then this check will pass, else this will cause test
 * failure.
 */
#define CHAOS_CHECK_TRUE( a )                                                  \
    if ( ( a ) )                                                               \
    {                                                                          \
        chaos::test::internal::TestCore::logger().report_check_pass(           \
                "CHAOS_CHECK_TRUE", __FILE__, __LINE__ );                      \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        chaos::test::internal::TestCore::logger().report_check_fail(           \
                "CHAOS_CHECK_TRUE", __FILE__, __LINE__, "" );                  \
    }

/*!
 * \brief Checks whether the given value evaluates to false.
 *
 * If a evaluates to false then this check will pass, else this will cause test
 * failure.
 */
#define CHAOS_CHECK_FALSE( a )                                                 \
    if ( !( a ) )                                                              \
    {                                                                          \
        chaos::test::internal::TestCore::logger().report_check_pass(           \
                "CHAOS_CHECK_FALSE", __FILE__, __LINE__ );                     \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        chaos::test::internal::TestCore::logger().report_check_fail(           \
                "CHAOS_CHECK_FALSE", __FILE__, __LINE__, "" );                 \
    }

/*!
 * \brief Checks whether the given values are considered equal.
 *
 * If a and b are equal this check will pass, else this will cause test failure.
 */
#define CHAOS_CHECK_EQUAL( a, b )                                              \
    {                                                                          \
    auto _a = (a); auto _b = _a; _b = (b);                                     \
    if ( _a == _b )                                                            \
    {                                                                          \
        chaos::test::internal::TestCore::logger().report_check_pass(           \
                "CHAOS_CHECK_EQUAL", __FILE__, __LINE__ );                     \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        chaos::str::UTF8String f_e_m;                                          \
        f_e_m << _a << " does not equal " << _b;                               \
        chaos::test::internal::TestCore::logger().report_check_fail(           \
                "CHAOS_CHECK_EQUAL", __FILE__, __LINE__, f_e_m );              \
    }                                                                          \
    }

/*!
 * \brief Checks whether the given value are considered not equal.
 *
 * If a and b are not equal this check will pass, else this will cause test
 * failure.
 */
#define CHAOS_CHECK_NOT_EQUAL( a, b )                                          \
    {                                                                          \
    auto _a = (a); auto _b = _a; _b = (b);                                     \
    if ( _a != _b )                                                            \
    {                                                                          \
        chaos::test::internal::TestCore::logger().report_check_pass(           \
                "CHAOS_CHECK_NOT_EQUAL", __FILE__, __LINE__ );                 \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        chaos::str::UTF8String f_e_m;                                          \
        f_e_m << _a << " equals " << _b;                                       \
        chaos::test::internal::TestCore::logger().report_check_fail(           \
                "CHAOS_CHECK_NOT_EQUAL", __FILE__, __LINE__, f_e_m );          \
    }                                                                          \
    }

/*!
 * \brief Checks whether the given float values are considered equal.
 *
 * If a and b are equal this check will pass, else this will cause test failure.
 * Equality is checked using chaos::math::float_equals with the default values
 * for ```delta_threshold``` and ```ulps_threshold```.
 */
#define CHAOS_CHECK_FLOAT_EQUAL( a, b )                                        \
    {                                                                          \
    auto _a = (a); auto _b = _a; _b = (b);                                     \
    if ( chaos::math::float_equals( _a, _b ) )                                 \
    {                                                                          \
        chaos::test::internal::TestCore::logger().report_check_pass(           \
                "CHAOS_CHECK_FLOAT_EQUAL", __FILE__, __LINE__ );               \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        chaos::str::UTF8String f_e_m;                                          \
        f_e_m << _a << " does not equal " << _b;                               \
        chaos::test::internal::TestCore::logger().report_check_fail(           \
                "CHAOS_CHECK_FLOAT_EQUAL", __FILE__, __LINE__, f_e_m );        \
    }                                                                          \
    }

/*!
 * \brief Checks whether the given float value are considered not equal.
 *
 * If a and b are not equal this check will pass, else this will cause test
 * failure.  * Equality is checked using chaos::math::float_equals with the
 * default values for ```delta_threshold``` and ```ulps_threshold```.
 */
#define CHAOS_CHECK_FLOAT_NOT_EQUAL( a, b )                                    \
    {                                                                          \
    auto _a = (a); auto _b = _a; _b = (b);                                     \
    if ( !chaos::math::float_equals( _a, _b ) )                                \
    {                                                                          \
        chaos::test::internal::TestCore::logger().report_check_pass(           \
                "CHAOS_CHECK_FLOAT_NOT_EQUAL", __FILE__, __LINE__ );           \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        chaos::str::UTF8String f_e_m;                                          \
        f_e_m << _a << " does not equal " << _b;                               \
        chaos::test::internal::TestCore::logger().report_check_fail(           \
                "CHAOS_CHECK_FLOAT_EQUAL", __FILE__, __LINE__, f_e_m );        \
    }                                                                          \
    }

/*!
 * \brief Checks whether the given statement throws the exception type.
 *
 * If the statement throws the given exception type the check will pass, else
 * this will cause test failure.
 */
#define CHAOS_CHECK_THROW( statement, exception_type )                         \
    {                                                                          \
    bool caught = false;                                                       \
    try                                                                        \
    {                                                                          \
        ( statement );                                                         \
    }                                                                          \
    catch( exception_type e )                                                  \
    {                                                                          \
        caught = true;                                                         \
        chaos::test::internal::TestCore::logger().report_check_pass(           \
                "CHAOS_CHECK_THROW", __FILE__, __LINE__ );                     \
    }                                                                          \
    catch( ... ) {}                                                            \
    if ( !caught )                                                             \
    {                                                                          \
        chaos::str::UTF8String f_e_m;                                          \
        f_e_m << "Exception type: " << #exception_type << " not thrown";       \
        chaos::test::internal::TestCore::logger().report_check_fail(           \
                "CHAOS_CHECK_THROW", __FILE__, __LINE__, f_e_m );              \
    }                                                                          \
    }

} // namespace test
} // namespace chaos

// reset the current module
namespace chaos_test_include
{
static chaos::test::internal::TestCore reset( "", NULL, "", 0, true );
}

#endif
