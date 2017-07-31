#include <QApplication>
#include <iostream>
#include <window.h>
#include <file_reader.hpp>
#include <parser_csv.hpp>
#include <fnn_math.hpp>

namespace {
    class App {
        int m_argc;
        char** m_argv;

        public:
        App(int argc, char** argv)
            : m_argc(argc), m_argv(argv) {}

        static const std::string& getVersion() {
            static const std::string ver = "1.0";
            return ver;
        }

        int run() {
            try {
                parsers::ParserCSV parser;
                file_reader::FileReader<parsers::ParserCSV> freader(&parser);
                const std::string filename = (m_argc > 1) ? m_argv[1] : "<empty>";
                freader.readFile(filename);

                auto& dataV = parser.getData();
                //dataV.print();

                QApplication app(m_argc, m_argv);
                Window graph;
                //graph.setData(&dataV, { 1, 2, 3, 4 }, filename);
                graph.setData(&dataV, { 1, 4 }, filename);
                graph.show();


                mw::DataVector dataVNorm;
                dataVNorm.setNames(dataV.getNames());
                math::compute<math::GaussNorm>(dataV, dataVNorm);
                //dataVNorm.print();
                Window ngraph;
                ngraph.setData(&dataVNorm, { 1, 4 }, "Gaussian normalisation");
                ngraph.show();

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

