#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(data)

#include <chaoscore/io/sys/FileReader.hpp>

#include <json/json.h>

#define META_WARN_ON_FALLBACK
#include <metaeng/Data.hpp>

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

    // metaeng::Data data(meta_path);
    metaeng::Data data(meta_path, &fallback);

    // chaos::uint8 remove_me;
    // remove_me = data.get("remove_me", remove_me);
    // std::cout << "remove_me: " << remove_me << std::endl;

    // std::vector<chaos::uint8> remove_me;
    // remove_me = data.get("remove_me", remove_me);
    // CHAOS_PRINT_ITERABLE(remove_me);


    // std::vector<chaos::str::UTF8String> resource_path_v;
    // resource_path_v = data.get("resource_path", resource_path_v);
    // CHAOS_PRINT_ITERABLE(resource_path_v);

    chaos::io::sys::Path resource_path;
    data.get("fonts_resource_path", resource_path);
    std::cout << "resource_path: " << resource_path.to_native() << std::endl;
}

} // namespace
