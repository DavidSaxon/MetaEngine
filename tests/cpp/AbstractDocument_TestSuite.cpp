#include <arcanecore/test/ArcTest.hpp>

ARC_TEST_MODULE(AbstractDocument)

#include <arcanecore/base/Exceptions.hpp>

#include <json/json.h>

#include <metaengine/AbstractDocument.hpp>
#include <metaengine/Document.hpp>

// TODO: REMOVE ME
#include <iostream>

namespace
{

//------------------------------------------------------------------------------
//                              TEST IMPLEMENTATION
//------------------------------------------------------------------------------

class TestDocument : public metaengine::AbstractDocument
{
public:

    //-------------------------------CONSTRUCTOR--------------------------------

    TestDocument(const arc::str::UTF8String& json_data)
        :
        m_json_data(json_data)
    {
        reload();
    }

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void reload()
    {
        init(m_json_data);
    }

private:

    //----------------------------PRIVATE ATTRIBUTES----------------------------

    arc::str::UTF8String m_json_data;
};

class TestV : public metaengine::Visitor
{
public:

    const arc::str::UTF8String& operator*()
    {
        m_str = "fuck";
        return m_str;
    }

    virtual void retrieve(const Json::Value* value)
    {
    }

private:

    arc::str::UTF8String m_str;
};

//------------------------------------------------------------------------------
//                                      INIT
//------------------------------------------------------------------------------

class InitFixture : public arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    std::vector<arc::str::UTF8String> valid_data;
    std::vector<arc::str::UTF8String> invalid_data;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        valid_data.push_back("{}");
        valid_data.push_back("{    }");
        valid_data.push_back("{ \t   }");
        valid_data.push_back("{\"Hello\": \"World\"}");
        valid_data.push_back("{\"list\": [1, 2, 3, 4]}");
        valid_data.push_back("{\"nested\": {\"bool\": true}}");

        invalid_data.push_back("");
        invalid_data.push_back("__");
        invalid_data.push_back("{");
        invalid_data.push_back("{\"Hello:\" \"World\"}");
        invalid_data.push_back("{\"Hello\": \"World}");
        invalid_data.push_back("{\"nested\": {{}}}");
    }
};

ARC_TEST_UNIT_FIXTURE(init, InitFixture)
{
    ARC_TEST_MESSAGE("Checking valid data");
    ARC_CONST_FOR_EACH(data, fixture->valid_data)
    {
        // just construct documents, tests will fail if an exception is thrown
        bool construct_success = true;
        try
        {
            TestDocument document(*data);
        }
        catch(arc::ex::ParseError)
        {
            construct_success = false;
        }
        ARC_CHECK_TRUE(construct_success);
    }

    ARC_TEST_MESSAGE("Checking invalid data");
    ARC_CONST_FOR_EACH(data, fixture->invalid_data)
    {
        // parse error should be thrown
        ARC_CHECK_THROW(TestDocument(*data), arc::ex::ParseError);
    }


    arc::io::sys::Path dummy;
    metaengine::Document d(dummy);

    std::cout << "get: " << *d.get("test", TestV()) << std::endl;

}

//------------------------------------------------------------------------------
//                                   GET VALUE
//------------------------------------------------------------------------------



} // namespace anonymous
