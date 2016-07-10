#include <arcanecore/test/ArcTest.hpp>

ARC_TEST_MODULE(visitors.Primitive)

#include <metaengine/visitors/Primitive.hpp>

namespace
{

//------------------------------------------------------------------------------
//                                      BOOL
//------------------------------------------------------------------------------

class BoolVFixture : public arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    arc::str::UTF8String valid;
    arc::str::UTF8String invalid;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        valid = "{\"key_1\": true, \"key_2\": false}";
        invalid = "{\"key_1\": 12, \"key_2\": \"Hello world!\"}";
    }
};

ARC_TEST_UNIT_FIXTURE(bool_v, BoolVFixture)
{
    ARC_TEST_MESSAGE("Checking against valid data");
    {
        metaengine::Document doc(&fixture->valid);
        ARC_CHECK_EQUAL(
            *doc.get("key_1", metaengine::BoolV::instance()),
            true
        );
        ARC_CHECK_EQUAL(
            *doc.get("key_2", metaengine::BoolV::instance()),
            false
        );
    }

    ARC_TEST_MESSAGE("Checking against invalid data");
    {
        metaengine::Document doc(&fixture->invalid);
        ARC_CHECK_THROW(
            *doc.get("key_1", metaengine::BoolV::instance()),
            arc::ex::TypeError
        );
        ARC_CHECK_THROW(
            *doc.get("key_2", metaengine::BoolV::instance()),
            arc::ex::TypeError
        );
    }
}

//------------------------------------------------------------------------------
//                                  BOOL VECTOR
//------------------------------------------------------------------------------

class BoolVectorVFixture : public arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    arc::str::UTF8String valid;
    arc::str::UTF8String invalid;

    std::vector<bool> result_1;
    std::vector<bool> result_2;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        valid =
            "{"
            "   \"key_1\": [false],"
            "   \"key_2\": [true, false, true],"
            "   \"empty\": []"
            "}";
        invalid = "{\"key_1\": 12, \"key_2\": false}";

        result_1 = {false};
        result_2 = {true, false, true};
    }
};

ARC_TEST_UNIT_FIXTURE(bool_vector_v, BoolVectorVFixture)
{
    ARC_TEST_MESSAGE("Checking against valid data");
    {
        metaengine::Document doc(&fixture->valid);

        std::vector<bool> value_1(
            *doc.get("key_1", metaengine::BoolVectorV::instance()));
        ARC_CHECK_EQUAL(value_1.size(), fixture->result_1.size());
        for(std::size_t i = 0; i < value_1.size(); ++i)
        {
            ARC_CHECK_EQUAL(value_1[i], fixture->result_1[i]);
        }

        std::vector<bool> value_2(
            *doc.get("key_2", metaengine::BoolVectorV::instance()));
        ARC_CHECK_EQUAL(value_2.size(), fixture->result_2.size());
        for(std::size_t i = 0; i < value_2.size(); ++i)
        {
            ARC_CHECK_EQUAL(value_2[i], fixture->result_2[i]);
        }

        std::vector<bool> empty(
            *doc.get("empty", metaengine::BoolVectorV::instance()));
        ARC_CHECK_EQUAL(empty.size(), 0);
    }

    ARC_TEST_MESSAGE("Checking against invalid data");
    {
        metaengine::Document doc(&fixture->invalid);
        ARC_CHECK_THROW(
            *doc.get("key_1", metaengine::BoolVectorV::instance()),
            arc::ex::TypeError
        );
        ARC_CHECK_THROW(
            *doc.get("key_2", metaengine::BoolVectorV::instance()),
            arc::ex::TypeError
        );
    }
}

//------------------------------------------------------------------------------
//                                    INTEGER
//------------------------------------------------------------------------------

class IntVFixture : public arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    arc::str::UTF8String valid;
    arc::str::UTF8String invalid;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        valid = "{\"key_1\": 175, \"key_2\": -84748}";
        invalid = "{\"key_1\": false, \"key_2\": \"Hello world!\"}";
    }
};

ARC_TEST_UNIT_FIXTURE(int_v, IntVFixture)
{
    ARC_TEST_MESSAGE("Checking against valid data");
    {
        metaengine::Document doc(&fixture->valid);
        ARC_CHECK_EQUAL(
            *doc.get("key_1", metaengine::IntV<arc::uint32>::instance()),
            175
        );
        ARC_CHECK_EQUAL(
            *doc.get("key_2", metaengine::IntV<arc::int64>::instance()),
            -84748
        );
    }

    ARC_TEST_MESSAGE("Checking against invalid data");
    {
        metaengine::Document doc(&fixture->invalid);
        ARC_CHECK_THROW(
            *doc.get("key_1", metaengine::IntV<arc::uint32>::instance()),
            arc::ex::TypeError
        );
        ARC_CHECK_THROW(
            *doc.get("key_2", metaengine::IntV<arc::uint32>::instance()),
            arc::ex::TypeError
        );
    }
}

//------------------------------------------------------------------------------
//                                 INTEGER VECTOR
//------------------------------------------------------------------------------

class IntVectorV : public arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    arc::str::UTF8String valid;
    arc::str::UTF8String invalid;

    std::vector<arc::uint32> result_1;
    std::vector<arc::int64> result_2;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        valid =
            "{"
            "   \"key_1\": [0],"
            "   \"key_2\": [58, -459, 589458, -1928],"
            "   \"empty\": []"
            "}";
        invalid = "{\"key_1\": 12, \"key_2\": false}";

        result_1 = {0};
        result_2 = {58, -459, 589458, -1928};
    }
};

ARC_TEST_UNIT_FIXTURE(int_vector_v, IntVectorV)
{
    ARC_TEST_MESSAGE("Checking against valid data");
    {
        metaengine::Document doc(&fixture->valid);

        std::vector<arc::uint32> value_1(
            *doc.get("key_1", metaengine::IntVectorV<arc::uint32>::instance()));
        ARC_CHECK_EQUAL(value_1.size(), fixture->result_1.size());
        for(std::size_t i = 0; i < value_1.size(); ++i)
        {
            ARC_CHECK_EQUAL(value_1[i], fixture->result_1[i]);
        }

        std::vector<arc::int64> value_2(
            *doc.get("key_2", metaengine::IntVectorV<arc::int64>::instance()));
        ARC_CHECK_EQUAL(value_2.size(), fixture->result_2.size());
        for(std::size_t i = 0; i < value_2.size(); ++i)
        {
            ARC_CHECK_EQUAL(value_2[i], fixture->result_2[i]);
        }

        std::vector<char> empty(
            *doc.get("empty", metaengine::IntVectorV<char>::instance()));
        ARC_CHECK_EQUAL(empty.size(), 0);
    }

    ARC_TEST_MESSAGE("Checking against invalid data");
    {
        metaengine::Document doc(&fixture->invalid);
        ARC_CHECK_THROW(
            *doc.get("key_1", metaengine::IntVectorV<arc::int16>::instance()),
            arc::ex::TypeError
        );
        ARC_CHECK_THROW(
            *doc.get("key_2", metaengine::IntVectorV<arc::uint8>::instance()),
            arc::ex::TypeError
        );
    }
}

//------------------------------------------------------------------------------
//                                     FLOAT
//------------------------------------------------------------------------------

class FloatVFixture : public arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    arc::str::UTF8String valid;
    arc::str::UTF8String invalid;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        valid = "{\"key_1\": 3.14, \"key_2\": -73.02}";
        invalid = "{\"key_1\": false, \"key_2\": \"Hello world!\"}";
    }
};

ARC_TEST_UNIT_FIXTURE(float_v, FloatVFixture)
{
    ARC_TEST_MESSAGE("Checking against valid data");
    {
        metaengine::Document doc(&fixture->valid);
        ARC_CHECK_EQUAL(
            *doc.get("key_1", metaengine::FloatV<float>::instance()),
            3.14F
        );
        ARC_CHECK_EQUAL(
            *doc.get("key_2", metaengine::FloatV<double>::instance()),
            -73.02
        );
    }

    ARC_TEST_MESSAGE("Checking against invalid data");
    {
        metaengine::Document doc(&fixture->invalid);
        ARC_CHECK_THROW(
            *doc.get("key_1", metaengine::FloatV<double>::instance()),
            arc::ex::TypeError
        );
        ARC_CHECK_THROW(
            *doc.get("key_2", metaengine::FloatV<float>::instance()),
            arc::ex::TypeError
        );
    }
}

//------------------------------------------------------------------------------
//                                  FLOAT VECTOR
//------------------------------------------------------------------------------

class FloatVectorV : public arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    arc::str::UTF8String valid;
    arc::str::UTF8String invalid;

    std::vector<float> result_1;
    std::vector<double> result_2;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        valid =
            "{"
            "   \"key_1\": [0.0],"
            "   \"key_2\": [58.2, -0.0003, 1.233, -4839.23],"
            "   \"empty\": []"
            "}";
        invalid = "{\"key_1\": 12, \"key_2\": false}";

        result_1 = {0.0F};
        result_2 = {58.2, -0.0003, 1.233, -4839.23};
    }
};

ARC_TEST_UNIT_FIXTURE(float_vector_v, FloatVectorV)
{
    ARC_TEST_MESSAGE("Checking against valid data");
    {
        metaengine::Document doc(&fixture->valid);

        std::vector<float> value_1(
            *doc.get("key_1", metaengine::FloatVectorV<float>::instance()));
        ARC_CHECK_EQUAL(value_1.size(), fixture->result_1.size());
        for(std::size_t i = 0; i < value_1.size(); ++i)
        {
            ARC_CHECK_EQUAL(value_1[i], fixture->result_1[i]);
        }

        std::vector<double> value_2(
            *doc.get("key_2", metaengine::FloatVectorV<double>::instance()));
        ARC_CHECK_EQUAL(value_2.size(), fixture->result_2.size());
        for(std::size_t i = 0; i < value_2.size(); ++i)
        {
            ARC_CHECK_EQUAL(value_2[i], fixture->result_2[i]);
        }

        std::vector<float> empty(
            *doc.get("empty", metaengine::FloatVectorV<float>::instance()));
        ARC_CHECK_EQUAL(empty.size(), 0);
    }

    ARC_TEST_MESSAGE("Checking against invalid data");
    {
        metaengine::Document doc(&fixture->invalid);
        ARC_CHECK_THROW(
            *doc.get("key_1", metaengine::FloatVectorV<double>::instance()),
            arc::ex::TypeError
        );
        ARC_CHECK_THROW(
            *doc.get("key_2", metaengine::FloatVectorV<float>::instance()),
            arc::ex::TypeError
        );
    }
}

} // namespace anonymous
