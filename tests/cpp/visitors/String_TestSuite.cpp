#include <arcanecore/test/ArcTest.hpp>

ARC_TEST_MODULE(visitors.String)

#include <metaengine/visitors/String.hpp>

namespace
{

//------------------------------------------------------------------------------
//                                   UTF8STRING
//------------------------------------------------------------------------------

class UTF8StringVFixture : public arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    arc::str::UTF8String valid;
    arc::str::UTF8String invalid;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        valid = "{\"key_1\": \"\", \"key_2\": \"Hello world!\"}";
        invalid = "{\"key_1\": 12, \"key_2\": [\"list\", \"of\", \"strings\"]}";
    }
};

ARC_TEST_UNIT_FIXTURE(utf8string_v, UTF8StringVFixture)
{
    ARC_TEST_MESSAGE("Checking against valid data");
    {
        metaengine::Document doc(&fixture->valid);
        ARC_CHECK_EQUAL(
            *doc.get("key_1", metaengine::UTF8StringV::instance()),
            arc::str::UTF8String("")
        );
        ARC_CHECK_EQUAL(
            *doc.get("key_2", metaengine::UTF8StringV::instance()),
            arc::str::UTF8String("Hello world!")
        );
    }

    ARC_TEST_MESSAGE("Checking against invalid data");
    {
        metaengine::Document doc(&fixture->invalid);
        ARC_CHECK_THROW(
            *doc.get("key_1", metaengine::UTF8StringV::instance()),
            arc::ex::TypeError
        );
        ARC_CHECK_THROW(
            *doc.get("key_2", metaengine::UTF8StringV::instance()),
            arc::ex::TypeError
        );
    }
}

//------------------------------------------------------------------------------
//                               UTF8STRING VECTOR
//------------------------------------------------------------------------------

class UTF8StringVectorVFixture : public arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    arc::str::UTF8String valid;
    arc::str::UTF8String invalid;

    std::vector<arc::str::UTF8String> result_1;
    std::vector<arc::str::UTF8String> result_2;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        valid =
            "{"
            "   \"key_1\": [\"Hello\", \"world!\"],"
            "   \"key_2\": [\"1\", \"2\", \"3\"],"
            "   \"empty\": []"
            "}";
        invalid = "{\"key_1\": 12, \"key_2\": false}";

        result_1 = {
            arc::str::UTF8String("Hello"),
            arc::str::UTF8String("world!")
        };
        result_2 = {
            arc::str::UTF8String("1"),
            arc::str::UTF8String("2"),
            arc::str::UTF8String("3")
        };
    }
};

ARC_TEST_UNIT_FIXTURE(utf8string_vector_v, UTF8StringVectorVFixture)
{
    ARC_TEST_MESSAGE("Checking against valid data");
    {
        metaengine::Document doc(&fixture->valid);

        std::vector<arc::str::UTF8String> value_1(
            *doc.get("key_1", metaengine::UTF8StringVectorV::instance()));
        ARC_CHECK_EQUAL(value_1.size(), fixture->result_1.size());
        for(std::size_t i = 0; i < value_1.size(); ++i)
        {
            ARC_CHECK_EQUAL(value_1[i], fixture->result_1[i]);
        }

        std::vector<arc::str::UTF8String> value_2(
            *doc.get("key_2", metaengine::UTF8StringVectorV::instance()));
        ARC_CHECK_EQUAL(value_2.size(), fixture->result_2.size());
        for(std::size_t i = 0; i < value_2.size(); ++i)
        {
            ARC_CHECK_EQUAL(value_2[i], fixture->result_2[i]);
        }

        std::vector<arc::str::UTF8String> empty(
            *doc.get("empty", metaengine::UTF8StringVectorV::instance()));
        ARC_CHECK_EQUAL(empty.size(), 0);
    }

    ARC_TEST_MESSAGE("Checking against invalid data");
    {
        metaengine::Document doc(&fixture->invalid);
        ARC_CHECK_THROW(
            *doc.get("key_1", metaengine::UTF8StringVectorV::instance()),
            arc::ex::TypeError
        );
        ARC_CHECK_THROW(
            *doc.get("key_2", metaengine::UTF8StringVectorV::instance()),
            arc::ex::TypeError
        );
    }
}

} // namespace anonymous
