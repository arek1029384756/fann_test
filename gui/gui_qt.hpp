#ifndef GUI_QT
#define GUI_QT

#include <iostream>
#include <memory>
#include <window.h>
#include <gui_interface.hpp>

namespace gui {

    class GuiGraphQt : public GuiGraphInterfaceExt, public GuiGraphInterfaceInt {
        std::unique_ptr<gui::Window> m_graph;

        public:
        GuiGraphQt()
            : m_graph(new gui::Window(this)) {
        }

        virtual void foo() {
            std::cout << __func__ << "()" << std::endl;
            m_graph->show();
        }

        virtual void bar() {
            std::cout << __func__ << "()" << std::endl;
        }

    };
}

#endif
