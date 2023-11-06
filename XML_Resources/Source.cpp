#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>

class XMLResource {
private:
    std::string xml_data;

public:
    static std::unique_ptr<XMLResource> create() {
        return std::make_unique<XMLResource>();
    }

    bool load(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            xml_data = buffer.str();
            file.close();
            return true;
        }
        else {
            throw std::runtime_error("Error while loading XML");
        }
    }

    bool save(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << xml_data;
            file.close();
            return true;
        }
        else {
            throw std::runtime_error("Error while saving XML");
        }
    }

    class Iterator {
    private:
        XMLResource& xml_resource;
        size_t current_pos;
        std::string current_element;

    public:
        Iterator(XMLResource& resource) : xml_resource(resource), current_pos(0) {
            findNextElement();
        }

        bool hasNext() const {
            return current_pos < xml_resource.xml_data.size();
        }

        void next() {
            findNextElement();
        }

        bool hasPrevious() const {
            return current_pos > 0;
        }

        void previous() {
            findPreviousElement();
        }

        std::string getCurrentElement() const {
            return current_element;
        }

    private:
        void findNextElement() {
            size_t start = xml_resource.xml_data.find('<', current_pos);
            size_t end = xml_resource.xml_data.find('>', start);
            if (start != std::string::npos && end != std::string::npos) {
                current_element = xml_resource.xml_data.substr(start, end - start + 1);
                current_pos = end + 1;
            }
            else {
                current_element.clear();
            }
        }

        void findPreviousElement() {
            size_t start = xml_resource.xml_data.rfind('<', current_pos - 1);
            size_t end = xml_resource.xml_data.find('>', current_pos);
            if (start != std::string::npos && end != std::string::npos) {
                current_element = xml_resource.xml_data.substr(start, end - start + 1);
                current_pos = start;
            }
            else {
                current_element.clear();
            }
        }
    };

    Iterator find(const std::string& name) {
        XMLResource::Iterator it(*this);
        while (it.hasNext()) {
            if (it.getCurrentElement().find("<" + name) != std::string::npos) {
                return it;
            }
            it.next();
        }
        return it;
    }

    bool add(const std::string& element, XMLResource::Iterator position) {
        size_t pos = xml_data.find(position.getCurrentElement());
        if (pos != std::string::npos) {
            xml_data.insert(pos, element);
            return true;
        }
        return false; 
    }

    bool erase(XMLResource::Iterator position) {
        size_t pos = xml_data.find(position.getCurrentElement());
        if (pos != std::string::npos) {
            xml_data.erase(pos, position.getCurrentElement().size());
            return true;
        }
        return false; 
    }
};

int main(int argc, const char* argv[]) {
    try {
        auto resource = XMLResource::create();

        if (resource->load("tree.xml")) {
            std::cout << "XML loaded successfully" << std::endl;
        }
        else {
            std::cerr << "Error while loading XML" << std::endl;
            return EXIT_FAILURE;
        }

        if (resource->save("new_tree.xml")) {
            std::cout << "XML saved in new_tree.xml." << std::endl;
        }
        else {
            std::cerr << "Error while saving XML" << std::endl;
            return EXIT_FAILURE;
        }

        XMLResource::Iterator it(*resource);

        while (!it.getCurrentElement().empty()) {
            std::cout << "Current Element: " << it.getCurrentElement() << std::endl;
            it.next();
        }

        XMLResource::Iterator foundElement = resource->find("li"); 

        if (foundElement.hasNext()) {
            std::cout << "Element found: " << foundElement.getCurrentElement() << std::endl;
            std::string newElement = "<new_element>Content</new_element>";

            if (resource->add(newElement, foundElement)) {
                std::cout << "New element added successfully." << std::endl;
            }
            else {
                std::cout << "Error adding the new element." << std::endl;
            }

            if (resource->erase(foundElement)) {
                std::cout << "Element erased successfully." << std::endl;
            }
            else {
                std::cout << "Error erasing the element." << std::endl;
            }
        }
        else {
            std::cout << "Element not found." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
