#ifndef DATA_PROCESSOR
#define DATA_PROCESSOR

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <string>
#include <logger.h>

namespace data_processor {

    class DataProcessor {
        std::thread m_th;
        std::atomic<std::uint8_t> m_continue;
        mw::DataVector m_dataV;
        std::mutex m_dataMtx;
        gui::GuiGraphInterfaceExtSync* m_ifc;

        DataProcessor& operator=(const DataProcessor& other) = delete;
        DataProcessor& operator=(const DataProcessor&& other) = delete;

        void go() {
            //Temporary visualization test
            std::size_t i = 0;
            while(m_continue.load()) {
                if((i + 500) < m_dataV.getElementsSize()) {
                    auto inp = m_dataV.getChunk(i, i + 400);
                    auto out = m_dataV.getChunk(i + 50, i + 450);
                    auto chunkGraph = inp.getCopy();
                    chunkGraph.appendHorizontal(out.getCopy());
                    std::set<int> maskCh = { 1, 2, 3, 4, 7, 8, 9, 10 };
                    m_ifc->setData(chunkGraph, maskCh, std::string("Chunk: "));
                    ++i;
                    std::this_thread::sleep_for(std::chrono::microseconds(10000));
                } else {
                    i = 0;
                }
            }
        }

        void join() {
            if(m_th.joinable()) {
                m_th.join();
            }
        }

        public:
        DataProcessor(gui::GuiGraphInterfaceExtSync* const ifc)
            : m_ifc(ifc) {
            tout << __func__ << "(), this: " << this << std::endl;
        }

        virtual ~DataProcessor() {
            stop();
            tout << __func__ << "(), [thread stopped] this: " << this << std::endl;
        }

        void start() {
            if(!m_th.joinable()) {
                m_continue.store(1);
                m_th = std::thread([&]() { go(); });
            } else {
                throw std::runtime_error(std::string("Data processor thread start failed"));
            }
        }

        void stop() {
            m_continue.store(0);
            join();
        }

        //Lock lck protects m_dataV against unsynchronized access from outer threads only
        //Inner thread uses m_dataV directly, but m_dataV update is possible only when stopped
        void setData(const mw::DataVector& dataV) {
            std::unique_lock<std::mutex> lck(m_dataMtx);
            if(!m_th.joinable()) {
                m_dataV = dataV;
            }
        }

    };

}

#endif
