#ifndef GUI_PROGRESS_QT
#define GUI_PROGRESS_QT

#include <iostream>
#include <memory>
#include <progress_qt.h>
#include <gui_progress_interface.hpp>

namespace gui {

    class GuiProgressQt : public GuiProgressInterfaceExt, public GuiProgressInterfaceInt {
        std::unique_ptr<gui_qt::ProgressQt> m_progress;

        public:
        GuiProgressQt()
            : m_progress(new gui_qt::ProgressQt(this)) {
        }


        //External interface
        virtual void show() const {
            m_progress->show();
        }


        //Internal interface

    };
}

#endif
