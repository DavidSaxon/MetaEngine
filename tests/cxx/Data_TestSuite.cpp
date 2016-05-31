#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(data)

#include <chaoscore/io/sys/FileReader.hpp>

#include <json/json.h>

#define META_WARN_ON_FALLBACK
#include <meta/Data.hpp>

namespace
{

// TODO: fixture?

CHAOS_TEST_UNIT(constructor)
{
    chaos::str::UTF8String fallback(
        "{"
            "\"resource_path\": \"res\","
            "\"gui_resource_path\": \"{resource_path}/gui\","
            "\"fonts_resource_path\": \"{gui_resource_path}/fonts\","
            "\"fonts\":"
            "{"
            "    \"supported_formats\": [\"ttf\", \"otf\"]"
            "}"
        "}"

    );

    // open a file
    chaos::io::sys::Path meta_path;
    // meta_path << "tests" << "meta" << "test_data_1.json";
    meta_path << "tests" << "meta" << "sub_1" << "test_data_2.json";

    // meta::Data data(meta_path);
    meta::Data data(meta_path, &fallback);

    chaos::uint8 remove_me;
    remove_me = data.get("remove_me", remove_me);
    std::cout << "remove_me: " << remove_me << std::endl;

    // std::vector<chaos::uint8> remove_me;
    // remove_me = data.get("remove_me", remove_me);
    // CHAOS_PRINT_ITERABLE(remove_me);


    // std::vector<chaos::str::UTF8String> resource_path_v;
    // resource_path_v = data.get("resource_path", resource_path_v);

    // CHAOS_PRINT_ITERABLE(resource_path_v);
}

} // namespace
