#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(data)

#include <chaoscore/io/sys/FileReader.hpp>

#include <json/json.h>

#include <meta/Data.hpp>

namespace
{

// TODO: fixture?

CHAOS_TEST_UNIT(constructor)
{
    // open a file
    chaos::io::sys::Path meta_path;
    meta_path << "tests" << "meta" << "test_data_1.json";
    chaos::io::sys::FileReader meta_file(
        meta_path,
        chaos::io::sys::FileReader::ENCODING_DETECT,
        chaos::io::sys::FileReader::NEWLINE_UNIX
    );

    // read the data from the file
    const chaos::str::UTF8String::Opt assume_valid(
        chaos::str::UTF8String::Opt::SKIP_VALID_CHECK);
    chaos::str::UTF8String meta_data(assume_valid);
    meta_file.read(meta_data);

    Json::Value root;
    Json::Reader reader;
    bool parse_sucess = reader.parse(
        meta_data.get_raw(),
        meta_data.get_raw() + (meta_data.get_byte_length() - 1),
        root
    );

    std::cout << "parse success?? " << parse_sucess << std::endl;

    std::cout << "read: " << root.get("value_1", "bad").asString() << std::endl;
}

} // namespace
