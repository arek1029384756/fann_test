#ifndef DATA_PROCESSOR
#define DATA_PROCESSOR

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

namespace data_processor {

    class DataProcessor {
        std::thread m_th;

        void go() {
            for(int i = 0; i < 50; ++i) {
                std::cout << "i: " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        public:
        static DataProcessor& getInstance() {
            static DataProcessor dp;
            return dp;
        }

        DataProcessor() {

        }

        virtual ~DataProcessor() {
            if(m_th.joinable()) {
                m_th.join();    
            }
        }

        void start() {
            if(!m_th.joinable()) {
                m_th = std::thread([&]() { go(); });
            } else {
                throw std::runtime_error(std::string("Data processor thread start failed"));
            }
        }

    };

}

#endif
