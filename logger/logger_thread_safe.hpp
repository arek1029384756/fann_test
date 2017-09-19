#ifndef LOGGER_THREAD_SAFE
#define LOGGER_THREAD_SAFE

#include <ostream>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace logger {

    typedef std::ostream& (manip)(std::ostream& os);

    class Logger : public std::ostream {
        static std::mutex osMtx;
        static std::thread::id thID;
        static std::mutex thIDMtx;
        static std::condition_variable cvWait;
        std::ostream* m_os;

        void testAndSetID() const {
            std::unique_lock<std::mutex> lck(thIDMtx);

            if(thID == std::thread::id()) {
                thID = std::this_thread::get_id();
            } else if(thID != std::this_thread::get_id()) {
                cvWait.wait(lck, []{ return thID == std::thread::id(); });
                thID = std::this_thread::get_id();
            }

        }

        void resetID() const {
            {
                std::lock_guard<std::mutex> lck(thIDMtx);
                thID = std::thread::id();
            }
            cvWait.notify_all();
        }

        public:
        Logger(std::ostream* const os = 0)
            : m_os(os) {
        }

        template<typename T>
        Logger& operator<<(const T& x) {
            if(m_os) {
                testAndSetID();
                *m_os << x;
            }
            return *this;
        }

        Logger& operator<<(const manip* const x) {
            if(m_os) {
                *m_os << x;
                if(x == static_cast<manip*>(&std::endl)) {
                    resetID();
                }
            }
            return *this;
        }

    };
}

#endif
