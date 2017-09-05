#ifndef GUI_GRAPH_INTERFACE
#define GUI_GRAPH_INTERFACE

#include <set>
#include <string>
#include <data_format.hpp>

namespace gui {

    class GuiGraphInterfaceExt {
        public:
        virtual void setData(const mw::DataVector* const dataV, const std::set<int>& mask, const std::string& graphName) = 0;
        virtual void show() const = 0;
    };

    class GuiGraphInterfaceInt {
        public:
        virtual std::size_t graphDataLength() const = 0;
        virtual double graphDataMax() const = 0;
        virtual double graphDataMin() const = 0;
        virtual const std::string& graphName() const = 0;
        virtual const std::set<int>& dataMask() const = 0;
        virtual const mw::DataVector& dataVector() const = 0;
    };

}

#endif
