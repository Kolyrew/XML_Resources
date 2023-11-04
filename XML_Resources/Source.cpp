#include <iostream>
#include <string>
#include <pugixml.hpp>

class XMLResource {
private:
    pugi::xml_document xml_document;

    XMLResource() {}

public:
    static XMLResource* create() {
        return new XMLResource();
    }

    bool load(const std::string& filename) {
        pugi::xml_parse_result result = xml_document.load_file(filename.c_str());
        return result.status == pugi::status_ok;
    }

    bool save(const std::string& filename) {
        return xml_document.save_file(filename.c_str());
    }
};

int32_t main(const int32_t argc, const char** const argv[]) {

    XMLResource* resource = XMLResource::create();

    if (resource->load("tree.xml")) {
        std::cout << "XML loaded succesfully" << std::endl;
    }
    else {
        std::cout << "Error while loading XML" << std::endl;
        return EXIT_FAILURE;
    }


    if (resource->save("new_tree.xml")) {
        std::cout << "XML saved in new_tree.xml." << std::endl;
    }
    else {
        std::cout << "Error while loading XML" << std::endl;
        return EXIT_FAILURE;
    }

    delete resource;

    return EXIT_SUCCESS;
}
