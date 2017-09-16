#ifndef DATA_PROCESSOR
#define DATA_PROCESSOR

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <string>

namespace data_processor {

    class DataProcessor {
        std::thread m_th;
        std::atomic<uint8_t> m_continue;

        void go() {
            int i = 0;
            while(m_continue.load()) {
                std::cout << "i: " << i++ << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        void join() {
            if(m_th.joinable()) {
                m_th.join();
            }
        }

        public:
        static DataProcessor& getInstance() {
            static DataProcessor dp;
            return dp;
        }

        DataProcessor() {
            std::cout << __func__ << "(), this: " << this << std::endl;
        }

        virtual ~DataProcessor() {
            std::cout << __func__ << "(), this: " << this << std::endl;
            stop();
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
    };

}

#endif
