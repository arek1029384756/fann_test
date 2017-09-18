#ifndef PARSER_CSV
#define PARSER_CSV

#include <iostream>
#include <fstream>
#include <algorithm>
#include <logger.h>
#include "parser_interface.hpp"

namespace parsers {

    class ParserCSV : public ParserInterface {

        static constexpr std::size_t descriptionRow = 0;

        static constexpr std::size_t descriptionCol = 0;

        mw::DataVector m_dataV;

        void setTokenData(const std::vector<std::string>& tokens) {
            try {
                mw::DataElement el;
                for(const auto& t : tokens) {
                    auto db = std::stod(t);
                    el.addData(db);
                }
                m_dataV.addElement(el);
            } catch(const std::invalid_argument& e) {
                for(const auto& t : tokens) {
                    m_dataV.addName(t);
                    tout << t << std::endl;
                }
                tout << "Processed non-number tokens" << std::endl;
            }
        }

        void lengthMismatchWarning(std::size_t tokensSize) const {
            static std::size_t currentSize = 0;
            static uint64_t lineCnt = 0;

            if(lineCnt > 0 && currentSize != tokensSize) {
                tout << "\033[0;33mWarning! Data length mismatch at line "
                << lineCnt << " (" << currentSize << " -> " << tokensSize << ")\033[0m" << std::endl;
            }
            currentSize = tokensSize;
            ++lineCnt;
        }

        public:
        ParserCSV() {
        }

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
                lengthMismatchWarning(tokens.size());
                setTokenData(tokens);
            } catch(const std::out_of_range& e) {
                throw std::runtime_error(std::string("Line processing error! Maybe your CSV file lacks some columns? '") +
                        e.what() + std::string("'"));
            }
        }

        virtual const mw::DataVector& getData() const override {
            return m_dataV;
        }

    };
}

#endif
