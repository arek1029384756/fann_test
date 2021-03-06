#include <QApplication>
#include <iostream>
#include <logger.h>
#include <gui_graph_qt.hpp>
#include <gui_progress_qt.hpp>
#include <file_reader.hpp>
#include <parser_csv.hpp>
#include <fnn_math.hpp>
#include <data_processor.hpp>

#include <thread>
#include <chrono>

//Temporary test function
void fun(gui::GuiProgressInterfaceExtSync* const ifc) {
    for(int i = 0; i < 101; ++i) {
        ifc->updateProgress(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

namespace {
    class App {
        int m_argc;
        char** m_argv;
        QApplication* m_qtApp;

        std::thread m_th;
        std::unique_ptr<gui::GuiGraphQt> m_graphRaw;
        std::unique_ptr<gui::GuiGraphQt> m_graphGauss;
        std::unique_ptr<gui::GuiGraphQt> m_graphChunk;
        std::unique_ptr<gui::GuiProgressQt> m_progress;

        //DataProcessor unique_ptr hast to be declared _after_ all
        //the widgets it uses (destruction order)
        std::unique_ptr<data_processor::DataProcessor> m_dataProcessor;

        public:
        App(int argc, char** argv, QApplication* const qtApp)
            : m_argc(argc), m_argv(argv), m_qtApp(qtApp) {}

        virtual ~App() {
            tout << __func__ << "(), this: " << this << std::endl;
            if(m_th.joinable()) {
                m_th.join();
            }
        }

        static const std::string& getVersion() {
            static const std::string ver = "1.0";
            return ver;
        }

        int run() {
            try {
                std::setlocale(LC_NUMERIC, "C");

                const std::string filename = (m_argc > 1) ? m_argv[1] : "<empty>";

                parsers::ParserCSV parser;
                file_reader::FileReader<parsers::ParserInterface> freader(&parser);
                freader.readFile(filename);

                auto& dataV = parser.getData();
                //dataV.print();

                auto inp = dataV.getChunk(10, 410);
                auto out = dataV.getChunk(50, 450);
                //auto trainV = mw::AnnVector(inp, out);
                //trainV.print();

                auto diff = inp.getDiff();
                diff.print();

                auto chunkGraph = inp.getCopy();
                chunkGraph.appendHorizontal(out.getCopy());

                std::set<int> maskCh = { 2, 3, 8, 9 };
                m_graphChunk.reset(new gui::GuiGraphQt());
                gui::GuiGraphInterfaceExtSync* pGraphChunkExtSync = m_graphChunk.get();
                gui::GuiGraphInterfaceExt* pGraphChunkExt = m_graphChunk.get();
                pGraphChunkExtSync->setData(chunkGraph, maskCh, std::string("Chunk: ") + filename);
                pGraphChunkExt->show();
#if 1
                std::set<int> mask = { 2, 3 };

                mw::DataVector dataVNorm;
                dataVNorm.setNames(dataV.getNames());
                math::compute<math::GaussNorm>(dataV, dataVNorm, mask);
                //dataVNorm.print();

                m_graphRaw.reset(new gui::GuiGraphQt());
                gui::GuiGraphInterfaceExtSync* pGraphRawExtSync = m_graphRaw.get();
                gui::GuiGraphInterfaceExt* pGraphRawExt = m_graphRaw.get();
                pGraphRawExtSync->setData(dataV, mask, std::string("Raw: ") + filename);
                pGraphRawExt->show();

                m_graphGauss.reset(new gui::GuiGraphQt());
                gui::GuiGraphInterfaceExtSync* pGraphGaussExtSync = m_graphGauss.get();
                gui::GuiGraphInterfaceExt* pGraphGaussExt = m_graphGauss.get();
                pGraphGaussExtSync->setData(dataVNorm, mask, std::string("Gauss: ") + filename);
                pGraphGaussExt->show();

                m_progress.reset(new gui::GuiProgressQt());

                gui::GuiProgressInterfaceExt* pProgressExt = m_progress.get();
                gui::GuiProgressInterfaceExtSync* pProgressExtSync = m_progress.get();
                pProgressExt->show();

                m_th = std::thread(fun, pProgressExtSync);
#endif

                m_dataProcessor.reset(new data_processor::DataProcessor(pGraphChunkExtSync));
                m_dataProcessor->setData(dataV);
                m_dataProcessor->start();

                return m_qtApp->exec();
            } catch(const std::exception& e) {
                terr << "\033[0;31mException raised:" << std::endl;
                terr << e.what() << "\033[0m" << std::endl;
                return EXIT_FAILURE;
            } catch(...) {
                terr << "\033[0;31mUnknown exception! Something really bad happened\033[0m"
                    << std::endl;
                return EXIT_FAILURE;
            }
            return 0;
        }
    };
}

int main(int argc, char** argv) {
    QApplication qtApp(argc, argv);
    App app(argc, argv, &qtApp);
    return app.run();
}
