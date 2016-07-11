#include <arcanecore/test/ArcTest.hpp>

ARC_TEST_MODULE(visitors.Path)

#include <metaengine/visitors/Path.hpp>

namespace
{

//------------------------------------------------------------------------------
//                                      PATH
//------------------------------------------------------------------------------

class PathVFixture : public arc::test::Fixture
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    std::vector<arc::str::UTF8String> valid;
    std::vector<arc::io::sys::Path> results;

    std::vector<arc::str::UTF8String> invalid;

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    virtual void setup()
    {
        {
            arc::str::UTF8String v(
                "{"
                "   \"key\": []"
                "}"
            );
            valid.push_back(v);

            arc::io::sys::Path r;
            results.push_back(r);
        }

        {
            arc::str::UTF8String v(
                "{"
                "   \"key\": [\"/\", \"path\", \"from\", \"root\"]"
                "}"
            );
            valid.push_back(v);

            arc::io::sys::Path r;
            r << "/" << "path" << "from" << "root";
            results.push_back(r);
        }

        {
            arc::str::UTF8String v(
                "{"
                "   \"key\": [\"relative\", \"path\"]"
                "}"
            );
            valid.push_back(v);

            arc::io::sys::Path r;
            r << "relative" << "path";
            results.push_back(r);
        }

        {
            arc::str::UTF8String v(
                "{"
                "   \"ref_key_1\": \"single\","
                "   \"key\": [\"/\", \"referenced\", \"@{ref_key_1}\", "
                "             \"path\", \"@{ref_key_2}\"],"
                "   \"ref_key_2\": [\"double\", \"ref\"]"
                "}"
            );
            valid.push_back(v);

            arc::io::sys::Path r;
            r << "/" << "referenced" << "single" << "path" << "double" << "ref";
            results.push_back(r);
        }

        {
            arc::str::UTF8String v(
                "{"
                "   \"ref_key_0\": [\"sub\", \"ref\"],"
                "   \"ref_key_1\": [\"@{ref_key_0}\"],"
                "   \"key\": [\"origin\", \"@{ref_key_1}\", \"@{ref_key_2}\"],"
                "   \"ref_key_2\": [\"double\", \"@{ref_key_1}\"]"
                "}"
            );
            valid.push_back(v);

            arc::io::sys::Path r;
            r << "origin" << "sub" << "ref" << "double" << "sub" << "ref";
            results.push_back(r);
        }

        {
            arc::str::UTF8String i(
                "{"
                "   \"key\": 12"
                "}"
            );
            invalid.push_back(i);
        }

        {
            arc::str::UTF8String i(
                "{"
                "   \"key\": \"/literal/path.txt\""
                "}"
            );
            invalid.push_back(i);
        }

        {
            arc::str::UTF8String i(
                "{"
                "   \"key\": [\"/\", \"invalid\", false]"
                "}"
            );
            invalid.push_back(i);
        }

        {
            arc::str::UTF8String i(
                "{"
                "   \"bad_ref\": 12,"
                "   \"key\": [\"/\", \"invalid\", \"@{bad_ref}\"]"
                "}"
            );
            invalid.push_back(i);
        }

        {
            arc::str::UTF8String i(
                "{"
                "   \"bad_ref\": 3.14,"
                "   \"ref_key\": [\"bad\", \"@{bad_ref}\"],"
                "   \"key\": [\"/\", \"invalid\", \"@{bad_ref}\"]"
                "}"
            );
            invalid.push_back(i);
        }

        {
            arc::str::UTF8String i(
                "{"
                "   \"key\": [\"relative\", \"@{key}\"]"
                "}"
            );
            invalid.push_back(i);
        }

        {
            arc::str::UTF8String i(
                "{"
                "   \"cyclic\": [\"first\", \"@{key}\", \"third\"],"
                "   \"key\": [\"relative\", \"@{cyclic}\"]"
                "}"
            );
            invalid.push_back(i);
        }

        {
            arc::str::UTF8String i(
                "{"
                "   \"cyclic_1\": [\"first\", \"@{cyclic_2}\", \"third\"],"
                "   \"key\": [\"relative\", \"@{cyclic_1}\"],"
                "   \"cyclic_2\": [\"@{cyclic_1}\", \"element\"]"
                "}"
            );
            invalid.push_back(i);
        }

        {
            arc::str::UTF8String i(
                "{"
                "   \"ref\": \"ref_path\","
                "   \"cyclic_1\": [\"@{ref}\", \"@{cyclic_2}\", \"third\"],"
                "   \"key\": [\"relative\", \"@{cyclic_1}\"],"
                "   \"cyclic_2\": [\"element\", \"@{key}\"]"
                "}"
            );
            invalid.push_back(i);
        }

    }
};

ARC_TEST_UNIT_FIXTURE(path_v, PathVFixture)
{
    ARC_TEST_MESSAGE("Checking against valid data");
    for(std::size_t i = 0; i < fixture->valid.size(); ++i)
    {
        metaengine::Document doc(&fixture->valid[i]);
        ARC_CHECK_EQUAL(
            *doc.get("key", metaengine::PathV::instance()),
            fixture->results[i]
        );
    }

    ARC_TEST_MESSAGE("Checking against invalid data");
    ARC_CONST_FOR_EACH(data, fixture->invalid)
    {
        metaengine::Document doc(&(*data));
        ARC_CHECK_THROW(
            *doc.get("key", metaengine::PathV::instance()),
            arc::ex::TypeError
        );
    }
}

} // namespace anonymous
