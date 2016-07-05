#include <arcanecore/test/ArcTest.hpp>

ARC_TEST_MODULE(Document)

#include <arcanecore/base/Exceptions.hpp>

#include <json/json.h>

#include <metaengine/Document.hpp>

namespace
{

//------------------------------------------------------------------------------
//                                 LOAD FILE PATH
//------------------------------------------------------------------------------

class LoadFilePathFixture : public virtual arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    std::vector<arc::io::sys::Path> valid_paths;
    std::vector<arc::io::sys::Path> invalid_paths;
    std::vector<arc::io::sys::Path> non_json_paths;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        // valid paths
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "simple.json";
            valid_paths.push_back(p);
        }
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "hierarchy.json";
            valid_paths.push_back(p);
        }
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "sub" << "simple.json";
            valid_paths.push_back(p);
        }
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "sub" << "hierarchy.json";
            valid_paths.push_back(p);
        }

        // invalid paths
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "simple.txt";
            invalid_paths.push_back(p);
        }
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "does_not_exist.json";
            invalid_paths.push_back(p);
        }
        {
            arc::io::sys::Path p;
            p << "tests" << "does_not_exist" << "hierarchy.json";
            invalid_paths.push_back(p);
        }
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "does_not_exist" << "hierarchy.json";
            invalid_paths.push_back(p);
        }

        // non JSON paths
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "bad_1.json";
            non_json_paths.push_back(p);
        }
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "bad_2.txt";
            non_json_paths.push_back(p);
        }
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "sub" << "bad_3.csv";
            non_json_paths.push_back(p);
        }
        {
            arc::io::sys::Path p;
            p << "tests" << "meta" << "sub" << "bad_4.json";
            non_json_paths.push_back(p);
        }
    }
};

ARC_TEST_UNIT_FIXTURE(load_file_path, LoadFilePathFixture)
{
    ARC_TEST_MESSAGE("Checking loading valid files");
    ARC_CONST_FOR_EACH(it, fixture->valid_paths)
    {
        metaengine::Document doc(*it);
        ARC_CHECK_TRUE(doc.is_using_file_path());
        ARC_CHECK_FALSE(doc.is_using_memory());
        ARC_CHECK_TRUE(doc.has_valid_file_data());
        ARC_CHECK_FALSE(doc.has_valid_memory_data());
    }

    ARC_TEST_MESSAGE("Checking loading invalid file paths");
    ARC_CONST_FOR_EACH(it, fixture->invalid_paths)
    {
        ARC_CHECK_THROW(metaengine::Document(*it), arc::ex::InvalidPathError);
    }

    ARC_TEST_MESSAGE("Checking loading non-JSON files");
    ARC_CONST_FOR_EACH(it, fixture->non_json_paths)
    {
        ARC_CHECK_THROW(metaengine::Document(*it), arc::ex::ParseError);
    }
}

//------------------------------------------------------------------------------
//                                  LOAD MEMORY
//------------------------------------------------------------------------------

class LoadMemoryFixture : public virtual arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    std::vector<arc::str::UTF8String> valid;
    std::vector<arc::str::UTF8String> invalid;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        valid.push_back("{}");
        valid.push_back("{\"simple\": false}");
        valid.push_back(
            "{"
            "    \"value_1\": \"Hello world!\","
            "    \"value_2\": 175,"
            "    \"value_3\": 3.14"
            "}"
        );
        valid.push_back(
            "{"
            "    \"first_error\": 3,"
            "    \"resource_path\": [\"res\"],"
            "    \"g_resource_path\": [\"${resource_path}\", \"gui\"],"
            "    \"fonts_resource_path\": [\"${g_resource_path}\", \"fonts\"],"
            "    \"fonts\":"
            "    {"
            "        \"supported_formats\": [\"ttf\", \"otf\"]"
            "    },"
            "    \"bool_test\": true"
            "}"
        );

        invalid.push_back("");
        invalid.push_back("Hello World!");
        invalid.push_back("this,is,not,json\nnot,json,at,all");
        invalid.push_back(
            "{"
            "    \"first_error\": 3,"
            "    \"resource_path\": [\"res\"],"
            "    \"g_resource_path\": [\"${resource_path}\", \"gui\"],"
            "    \"fonts_resource_path\": [\"${g_resource_path}\", \"fonts\"],"
            "    \"fonts\":"
            "    {"
            "        \"supported_formats\":: [\"ttf\", \"otf\"]"
            "    },"
            "    \"bool_test\": true"
            "}"
        );
    }
};

ARC_TEST_UNIT_FIXTURE(load_memory, LoadMemoryFixture)
{
    ARC_TEST_MESSAGE("Checking loading valid JSON strings");
    ARC_CONST_FOR_EACH(it, fixture->valid)
    {
        metaengine::Document doc(&(*it));
        ARC_CHECK_FALSE(doc.is_using_file_path());
        ARC_CHECK_TRUE(doc.is_using_memory());
        ARC_CHECK_FALSE(doc.has_valid_file_data());
        ARC_CHECK_TRUE(doc.has_valid_memory_data());
    }

    ARC_TEST_MESSAGE("Checking loading invalid JSON strings");
    ARC_CONST_FOR_EACH(it, fixture->invalid)
    {
        ARC_CHECK_THROW(metaengine::Document(&(*it)), arc::ex::ParseError);
    }
}

//------------------------------------------------------------------------------
//                                 LOAD FALLBACK
//------------------------------------------------------------------------------

class LoadFallbackFixture : public LoadFilePathFixture, public LoadMemoryFixture
{
public:

    //-------------------------PUBLIC STATIC ATTRIBUTES-------------------------

    static bool report_callback;
    static arc::io::sys::Path report_file_path;
    static arc::str::UTF8String report_message;

    //----------------------------CALLBACK FUNCTIONS----------------------------

    static void reporter_func(
            const arc::io::sys::Path& file_path,
            const arc::str::UTF8String& message)
    {
        report_callback = true;
        report_file_path = file_path;
        report_message   = report_message;
    }

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        // super calls
        LoadFilePathFixture::setup();
        LoadMemoryFixture::setup();

        // hook up the callback
        metaengine::Document::set_load_fallback_reporter(reporter_func);
    }

    static void reset_state()
    {
        report_callback = false;
        report_file_path = arc::io::sys::Path();
        report_message = arc::str::UTF8String();
    }
};

bool LoadFallbackFixture::report_callback = false;
arc::io::sys::Path LoadFallbackFixture::report_file_path;
arc::str::UTF8String LoadFallbackFixture::report_message;

ARC_TEST_UNIT_FIXTURE(load_fallback, LoadFallbackFixture)
{
    ARC_TEST_MESSAGE("Checking all valid");
    for(std::size_t i = 0; i < fixture->valid_paths.size(); ++i)
    {
        metaengine::Document doc(fixture->valid_paths[i], &fixture->valid[i]);
        ARC_CHECK_FALSE(LoadFallbackFixture::report_callback);
        ARC_CHECK_TRUE(doc.is_using_file_path());
        ARC_CHECK_TRUE(doc.is_using_memory());
        ARC_CHECK_TRUE(doc.has_valid_file_data());
        ARC_CHECK_TRUE(doc.has_valid_memory_data());
    }
    LoadFallbackFixture::reset_state();

    ARC_TEST_MESSAGE("Checking valid path, invalid memory");
    for(std::size_t i = 0; i < fixture->valid_paths.size(); ++i)
    {
        metaengine::Document doc(fixture->valid_paths[i], &fixture->invalid[i]);
        ARC_CHECK_TRUE(LoadFallbackFixture::report_callback);
        ARC_CHECK_EQUAL(
            LoadFallbackFixture::report_file_path,
            fixture->valid_paths[i]
        );
        ARC_CHECK_TRUE(doc.is_using_file_path());
        ARC_CHECK_TRUE(doc.is_using_memory());
        ARC_CHECK_TRUE(doc.has_valid_file_data());
        ARC_CHECK_FALSE(doc.has_valid_memory_data());
    }
    LoadFallbackFixture::reset_state();

    ARC_TEST_MESSAGE("Checking invalid path, valid memory");
    for(std::size_t i = 0; i < fixture->invalid_paths.size(); ++i)
    {
        metaengine::Document doc(fixture->invalid_paths[i], &fixture->valid[i]);
        ARC_CHECK_TRUE(LoadFallbackFixture::report_callback);
        ARC_CHECK_EQUAL(
            LoadFallbackFixture::report_file_path,
            fixture->invalid_paths[i]
        );
        ARC_CHECK_TRUE(doc.is_using_file_path());
        ARC_CHECK_TRUE(doc.is_using_memory());
        ARC_CHECK_FALSE(doc.has_valid_file_data());
        ARC_CHECK_TRUE(doc.has_valid_memory_data());
    }
    LoadFallbackFixture::reset_state();

    ARC_TEST_MESSAGE("Checking invalid path, invalid memory");
    for(std::size_t i = 0; i < fixture->invalid_paths.size(); ++i)
    {
        ARC_CHECK_THROW(
            metaengine::Document(
                    fixture->invalid_paths[i],
                    &fixture->invalid[i]
            ),
            arc::ex::ParseError
        );
        ARC_CHECK_TRUE(LoadFallbackFixture::report_callback);
        ARC_CHECK_EQUAL(
            LoadFallbackFixture::report_file_path,
            fixture->invalid_paths[i]
        );
    }
    LoadFallbackFixture::reset_state();

    ARC_TEST_MESSAGE("Checking non-JSON path, valid memory");
    for(std::size_t i = 0; i < fixture->non_json_paths.size(); ++i)
    {
        metaengine::Document doc(
            fixture->non_json_paths[i],
            &fixture->valid[i]
        );
        ARC_CHECK_TRUE(LoadFallbackFixture::report_callback);
        ARC_CHECK_EQUAL(
            LoadFallbackFixture::report_file_path,
            fixture->non_json_paths[i]
        );
        ARC_CHECK_TRUE(doc.is_using_file_path());
        ARC_CHECK_TRUE(doc.is_using_memory());
        ARC_CHECK_FALSE(doc.has_valid_file_data());
        ARC_CHECK_TRUE(doc.has_valid_memory_data());
    }
    LoadFallbackFixture::reset_state();

    ARC_TEST_MESSAGE("Checking non-JSON path, invalid memory");
    for(std::size_t i = 0; i < fixture->non_json_paths.size(); ++i)
    {
        ARC_CHECK_THROW(
            metaengine::Document(
                    fixture->non_json_paths[i],
                    &fixture->invalid[i]
            ),
            arc::ex::ParseError
        );
        ARC_CHECK_TRUE(LoadFallbackFixture::report_callback);
        ARC_CHECK_EQUAL(
            LoadFallbackFixture::report_file_path,
            fixture->non_json_paths[i]
        );
    }
    LoadFallbackFixture::reset_state();
}

} // namespace anonymous
