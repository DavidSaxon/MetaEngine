#include <arcanecore/test/ArcTest.hpp>

ARC_TEST_MODULE(data)

#include <arcanecore/io/sys/FileReader.hpp>

#include <json/json.h>

#define META_WARN_ON_FALLBACK
#include <metaeng/Data.hpp>

namespace
{

// TODO: fixture?

// bool

ARC_TEST_UNIT(constructor)
{
    arc::str::UTF8String fallback(
        "{"
            "\"first_error\": 3.14,"
            "\"resource_path\": [\"res\"],"
            "\"gui_resource_path\": [\"${resource_path}\", \"gui\"],"
            "\"fonts_resource_path\": [\"${gui_resource_path}\", \"fonts\"],"
            "\"fonts\":"
            "{"
            "    \"supported_formats\": [\"ttf\", \"otf\"]"
            "}"
        "}"

    );

    // open a file
    arc::io::sys::Path meta_path;
    // meta_path << "tests" << "meta" << "test_data_1.json";
    meta_path << "tests" << "meta" << "sub_1" << "test_data_2.json";

    // metaeng::Data data(meta_path);
    metaeng::Data data(meta_path, fallback);

    // arc::uint8 remove_me;
    // remove_me = data.get("remove_me", remove_me);
    // std::cout << "remove_me: " << remove_me << std::endl;

    // std::vector<arc::uint8> remove_me;
    // remove_me = data.get("remove_me", remove_me);
    // ARC_PRINT_ITERABLE(remove_me);


    // std::vector<arc::str::UTF8String> resource_path_v;
    // resource_path_v = data.get("resource_path", resource_path_v);
    // ARC_PRINT_ITERABLE(resource_path_v);

    // arc::io::sys::Path resource_path;
    // data.get("fonts_resource_path", resource_path);
    // std::cout << "resource_path: " << resource_path.to_native() << std::endl;

    // bool test = bool();
    bool test = data.get("bool_test", test);
    std::cout << "test: " << test << std::endl;
}

} // namespace