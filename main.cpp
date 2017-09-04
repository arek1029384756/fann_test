#include <QApplication>
#include <iostream>
#include <gui_qt.hpp>
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
                QApplication app(m_argc, m_argv);
                std::setlocale(LC_NUMERIC, "C");

                const std::string filename = (m_argc > 1) ? m_argv[1] : "<empty>";

                parsers::ParserCSV parser;
                file_reader::FileReader<parsers::ParserCSV> freader(&parser);
                freader.readFile(filename);

                auto& dataV = parser.getData();
                //dataV.print();

                auto inp = dataV.getChunk(10, 15);
                auto out = dataV.getChunk(16, 18);
                auto trainV = mw::AnnVector(inp, out);
                trainV.print();

                auto diff = inp.getDiff();
                diff.print();


                std::set<int> mask = { 1, 4 };

                mw::DataVector dataVNorm;
                dataVNorm.setNames(dataV.getNames());
                math::compute<math::GaussNorm>(dataV, dataVNorm, mask);
                //dataVNorm.print();

                std::unique_ptr<gui::GuiGraphInterfaceExt> pG(new gui::GuiGraphQt());
                pG->foo();


#if 0
                gui::Window graph;
                graph.setData(&dataV, mask, std::string("Raw: ") + filename);
                graph.show();

                gui::Window ngraph;
                ngraph.setData(&dataVNorm, mask, std::string("Gauss: ") + filename);
                ngraph.show();
#endif

                return app.exec();
            } catch(const std::exception& e) {
                std::cerr << "\033[0;31mException raised:" << std::endl;
                std::cerr << e.what() << "\033[0m" << std::endl;
                return EXIT_FAILURE;
            } catch(...) {
                std::cerr << "\033[0;31mUnknown exception! Something really bad happened\033[0m"
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
