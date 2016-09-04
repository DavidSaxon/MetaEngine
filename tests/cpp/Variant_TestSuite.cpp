#include <arcanecore/test/ArcTest.hpp>

ARC_TEST_MODULE(Variant)

#include <json/json.h>

#include <metaengine/Variant.hpp>
#include <metaengine/visitors/Primitive.hpp>
#include <metaengine/visitors/String.hpp>

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
    }
};

//------------------------------------------------------------------------------
//                                     PROTO
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(proto, LoadFilePathFixture)
{
    arc::io::sys::Path v_path;
    v_path << "tests" << "meta" << "variants" << "_1" << "lang.json";
    metaengine::Variant v(v_path, "uk", true);

    std::cout << "uk get: "
              << (*v.get("hello_world", metaengine::UTF8StringV::instance()))
              << std::endl;

    v.set_variant("de");

    std::cout << "de get: "
              << (*v.get("hello_world", metaengine::UTF8StringV::instance()))
              << std::endl;

    v.set_variant("ko");

    std::cout << "ko get: "
              << (*v.get("hello_world", metaengine::UTF8StringV::instance()))
              << std::endl;
}

} // namespace anonymous
