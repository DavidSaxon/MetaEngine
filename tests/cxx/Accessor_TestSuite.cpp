#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(accessor)

#include <memory>

#include <meta/Accessor.hpp>

namespace
{

class ConstructorFixture : public chaos::test::Fixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    std::vector<chaos::io::sys::Path> paths;
    std::vector<bool> directories;

    //-----------------------------MEMBER FUNCTIONS-----------------------------

    virtual void setup()
    {
        // generate test data
        {
            chaos::io::sys::Path p;
            p << "tests" << "meta";
            paths.push_back(p);
            directories.push_back(true);
        }

        {
            chaos::io::sys::Path p;
            p << "tests" << "meta" << "test_data_1.meta";
            paths.push_back(p);
            directories.push_back(false);
        }

        {
            chaos::io::sys::Path p;
            p << "tests" << "meta" << "sub_1";
            paths.push_back(p);
            directories.push_back(true);
        }
    }
};

CHAOS_TEST_UNIT_FIXTURE(constructor, ConstructorFixture)
{
    std::vector<std::unique_ptr<meta::Accessor>> accessors;
    CHAOS_FOR_EACH(it, fixture->paths)
    {
        accessors.push_back(std::unique_ptr<meta::Accessor>(
            new meta::Accessor(*it)));
    }

    CHAOS_TEST_MESSAGE("Checking get_path");
    for (std::size_t i = 0; i < accessors.size(); ++i)
    {
        CHAOS_CHECK_EQUAL(accessors[i]->get_path(), fixture->paths[i]);
    }

    CHAOS_TEST_MESSAGE("Checking is_directory");
    for (std::size_t i = 0; i < accessors.size(); ++i)
    {
        CHAOS_CHECK_EQUAL(accessors[i]->get_path(), fixture->paths[i]);
    }
}

} // namespace anonymous
