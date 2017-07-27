#include <QApplication>
#include <iostream>
#include <window.h>
#include <file_reader.hpp>
#include <parser_csv.hpp>

namespace {
    class App {
        int m_argc;
        char** m_argv;
#if 0
        void showUsage() const {
            std::cout << "BacKup CoPy ver " << getVersion() << std::endl;
            std::cout << "Local safety copy utility, (C) A.Antoniak" << std::endl;
            std::cout << "Usage:" << std::endl;
            std::cout << "    " << m_argv[0] << " COMMAND [DIRECTORY]" << std::endl << std::endl;
            std::cout << "    COMMAND     - { \"nosilo status\", \"git status\" }" << std::endl;
            std::cout << "    DIRECTORY   - destination (leave empty for current directory)" << std::endl;
        }

        void checkOptions() const {
            if(m_argc < 2 || std::string(m_argv[1]) == "--help") {
                showUsage();
                throw std::runtime_error("");
            }
        }

        status_reader::StatusReader* checkVersionControl() const {
            checkOptions();

            auto pos = std::string(m_argv[1]).find("silo");
            if(pos != std::string::npos) {
                return status_reader::NsStatusReader::getInstance();
            } else {
                return status_reader::GitStatusReader::getInstance();
            }
        }
#endif
        public:
        App(int argc, char** argv)
            : m_argc(argc), m_argv(argv) {}

        static const std::string& getVersion() {
            static const std::string ver = "1.0";
            return ver;
        }

        int run() {
            try {
                parsers::ParserCSV parser(4);
                file_reader::FileReader<parsers::ParserCSV> freader(&parser);
                const std::string filename = (m_argc > 1) ? m_argv[1] : "<empty>";
                freader.readFile(filename);

                auto data = parser.getData();
                /*for(const auto& x : data) {
                    std::cout << x << std::endl;
                }*/

                QApplication app(m_argc, m_argv);
                Window graph;
                graph.setData(&data, filename);
                graph.show();
                return app.exec();
            } catch(const std::exception& e) {
                std::cout << "Exception raised:" << std::endl;
                std::cout << e.what() << std::endl;
                return EXIT_FAILURE;
            } catch(...) {
                std::cout << "Unknown exception! Something really bad happened"
                    << std::endl;
                return EXIT_FAILURE;
            }
            return 0;
        }
    };
}

int main(int argc, char** argv) {
    App a(argc, argv);
    return a.run();
}

