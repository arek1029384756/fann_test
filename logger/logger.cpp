#include <logger_thread_safe.hpp>

namespace logger {
    std::mutex Logger::osMtx;
    std::thread::id Logger::thID;
    std::mutex Logger::thIDMtx;
    std::condition_variable Logger::cvWait;
}

#if defined (_SILENT)
logger::Logger tout;
logger::Logger terr;
#elif defined (_VERBOSE)
logger::Logger tout(&std::cout);
logger::Logger terr(&std::cerr);
#else
    #error "Unknown logging mode"
#endif
