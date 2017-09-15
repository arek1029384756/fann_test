#ifndef DATA_FORMAT
#define DATA_FORMAT

#include <algorithm>
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

        double dataAt(std::size_t idx) const {
            return m_data.at(idx);
        }

        std::size_t dataSize() const {
            return m_data.size();
        }

        void appendData(const DataElement& other) {
            m_data.insert(m_data.end(), other.getData().begin(), other.getData().end());
        }

        void print() const {
            auto& v = getData();
            for(auto val : v) {
                std::cout << val << "\t\t";
            }
            std::cout << std::endl;
        }

        DataElement operator-(const DataElement& other) const {
            DataElement el;
            for(std::size_t i = 0; i < std::min(dataSize(), other.dataSize()); ++i) {
                el.addData(dataAt(i) - other.dataAt(i));
            }
            return el;
        }
    };

    template<typename TDataVector, typename TElement>
    class DataChunk {
        const TDataVector* m_dataV;
        std::size_t m_idxFirst;
        std::size_t m_idxLast;
        typename std::vector<TElement>::const_iterator m_first;
        typename std::vector<TElement>::const_iterator m_last;

        public:
        DataChunk(const TDataVector* const dataV, std::size_t first, std::size_t last)
            : m_dataV(dataV),
            m_idxFirst(first),
            m_idxLast(last),
            m_first(std::next(m_dataV->getElements().cbegin(), first)),
            m_last(std::next(m_dataV->getElements().cbegin(), last)) {
        }

        TElement getDiff() const {
            auto& elements = m_dataV->getElements();
            return elements.at(m_idxLast) - elements.at(m_idxFirst);
        }

        //Makes new data vector as a copy of the chunk
        //This method makes a copy of potentially large data structure
        //Use with caution! Only for helper purposes (not for data processing)
        TDataVector getCopy() const {
            TDataVector v;

            v.setNames(m_dataV->getNames());
            for(auto it = m_first; it != std::next(m_last, 1); std::advance(it, 1)) {
                v.addElement(*it);
            }

            return v;
        }

        void print() const {
            std::cout << "Chunk[" << m_idxFirst << ", " << m_idxLast << "]" << std::endl;
            for(auto it = m_first; it != std::next(m_last, 1); std::advance(it, 1)) {
                it->print();
            }
            std::cout << std::endl;
        }
    };

    class DataVector {
        std::vector<std::string> m_names;

        std::vector<DataElement> m_elements;

        public:
        typedef DataChunk<DataVector, DataElement> DVChunk;

        DataVector() {
            std::cout << __func__ << "(), this: " << this << std::endl;
        }

        const std::vector<DataElement>& getElements() const {
            return m_elements;
        }

        std::size_t getElementsSize() const {
            return m_elements.size();
        }

        const DataElement& getElementAt(std::size_t idx) const {
            return m_elements.at(idx);
        }

        const std::vector<std::string>& getNames() const {
            return m_names;
        }

        const std::string& getNameAt(std::size_t idx) const {
            return m_names.at(idx);
        }

        std::size_t elementDataSize() const {
            return m_elements.at(0).dataSize();
        }

        void addElement(const DataElement& elem) {
            m_elements.emplace_back(elem);
        }

        void addName(const std::string& name) {
            m_names.emplace_back(name);
        }

        void setNames(const std::vector<std::string>& names) {
            m_names = names;
        }

        void appendHorizontal(const DataVector& other) {
            m_names.insert(m_names.end(), other.getNames().begin(), other.getNames().end());

            for(std::size_t i = 0; i < std::min(getElementsSize(), other.getElementsSize()); ++i) {
                auto& element = m_elements.at(i);
                element.appendData(other.getElementAt(i));
            }
        }

        void print() const {
            for(const auto& n : m_names) {
                std::cout << n << "\t";
            }
            std::cout << std::endl;

            for(const auto& el : m_elements) {
                el.print();
            }
        }

        DVChunk getChunk(std::size_t first, std::size_t last) const {
            return DVChunk(this, first, last);
        }
    };

    class AnnVector {
        DataVector::DVChunk m_input;
        DataVector::DVChunk m_output;

        public:
        AnnVector(const DataVector::DVChunk& input, const DataVector::DVChunk& output)
            : m_input(input), m_output(output) {
        }

        const DataVector::DVChunk input() const {
            return m_input;
        }

        const DataVector::DVChunk output() const {
            return m_output;
        }

        void print() const {
            std::cout << std::endl << "Input ";
            m_input.print();
            std::cout << std::endl << "Output ";
            m_output.print();
        }
    };

}

#endif
