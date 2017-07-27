#ifndef FILE_READER
#define FILE_READER

#include <fstream>
#include <string>
#include <vector>

namespace file_reader {

    class FileReader {

        void removeWindowsDelim(std::string& line) {
            try {
                auto last = line.length() - 1;
                if(line.at(last) == 0x0D) {
                    line.erase(last, 1); 
                }
            } catch(const std::out_of_range& e) {
                throw std::runtime_error(std::string("File processing error! Maybe your file has some empty lines? '") +
                        e.what() + std::string("'"));
            }
        }

        protected:
        virtual void parseLine(const std::string& line) = 0;

        public:
        virtual const std::vector<double>& getData() const = 0;

        void readFile(const std::string& filename) {
            std::ifstream ifs(filename);
            if(ifs.is_open()) {
                std::string line;
                while(std::getline(ifs, line)) {
                    removeWindowsDelim(line);
                    parseLine(line);
                }
            } else {
                throw std::runtime_error(std::string("File open error '") + filename + std::string("'"));
            }
        }
    };
}

#endif
