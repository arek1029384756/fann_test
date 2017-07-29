#ifndef DATA_FORMAT
#define DATA_FORMAT

#include <iostream>
#include <string>
#include <vector>

namespace mw {

    class DataElement {
        std::string m_description;

        std::vector<double> m_data;

        public:
        const std::vector<double>& getData() const {
            return m_data;
        }

        void addData(double d) {
            m_data.emplace_back(d);
        }

        std::size_t dataSize() const {
            return m_data.size();
        }
    };

    class DataVector {
        std::vector<std::string> m_names;

        std::vector<DataElement> m_elements;

        public:
        const std::vector<DataElement>& getElements() const {
            return m_elements;
        }

        const std::vector<std::string>& getNames() const {
            return m_names;
        }

        std::size_t elementDataSize() const {
            return m_elements[0].dataSize();
        }

        void addElement(const DataElement& elem) {
            m_elements.emplace_back(elem);
        }

        void addName(const std::string& name) {
            m_names.emplace_back(name);
        }

        void print() {
            for(const auto& n : m_names) {
                std::cout << n << "\t";
            }
            std::cout << std::endl;

            for(const auto& el : m_elements) {
                auto v = el.getData();
                for(auto val : v) {
                    std::cout << val << "\t\t";
                }
                std::cout << std::endl;
            }
        }
    };
}

#endif
