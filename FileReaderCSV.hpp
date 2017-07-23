#ifndef FILE_READER_CSV
#define FILE_READER_CSV

#include <iostream>
#include <fstream>
#include <algorithm>
#include "FileReader.hpp"

namespace file_reader {

    class FileReaderCSV : public FileReader {

        std::size_t m_csvColumn;

        std::vector<double> m_data;

        FileReaderCSV() {}

        FileReaderCSV(std::size_t column = 0)
            : m_csvColumn(column) {
        }

        void setTokenData(const std::string& token) {
            try {
                auto db = std::stod(token);
                m_data.emplace_back(db);
            } catch(const std::invalid_argument& e) {
                //ignore non-number tokens
                std::cout << "Ignoring non-number tokens" << std::endl;
            }
        }

        protected:
        virtual void parseLine(const std::string& line) override {
            try {
                std::vector<std::string> tokens;
                std::size_t itb = 0, itc;
                do {
                    itc = line.find(',', itb);
                    auto s = line.substr(itb, itc - itb);
                    tokens.emplace_back(s);
                    itb = itc + 1;
                } while(itc != std::string::npos);
                setTokenData(tokens.at(m_csvColumn));
            } catch(const std::out_of_range& e) {
                throw std::runtime_error(std::string("Line processing error! Maybe your CSV file lacks some columns? '") +
                        e.what() + std::string("'"));
            }
        }

        public:
        void setColumn(std::size_t column) {
            m_csvColumn = column;
        }

        const std::vector<double>& getData() const {
            return m_data;
        }

        static FileReader* getInstance(std::size_t column = 0) {
            static FileReaderCSV n(column);
            return &n;
        }
 
    };
}

#endif
