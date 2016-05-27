// TODO: use paths
meta::set_directory("tests/meta/");
resource_data = meta::retrieve_file("sub_1/test_data_2.json")

font_data = resource_data["fonts", false];

// TODO: can this be un-overloaded
resource_data["fonts"].get<std::vector<std::string>>("supported_formats");
