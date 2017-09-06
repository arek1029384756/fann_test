#ifndef GUI_PROGRESS_QT
#define GUI_PROGRESS_QT

#include <iostream>
#include <memory>
#include <atomic>
#include <progress_qt.h>
#include <gui_progress_interface.hpp>

namespace gui {

    class GuiProgressQt : public GuiProgressInterfaceExtSync,
                            public GuiProgressInterfaceExt,
                            public GuiProgressInterfaceInt {
        std::unique_ptr<gui_qt::ProgressQt> m_progress;

        std::atomic<int> m_value;

        void notifyProgress() const {
            emit m_progress->signalNewData();
        }

        public:
        GuiProgressQt()
            : m_progress(new gui_qt::ProgressQt(this)) {
            std::cout << "Atomic progress bar data update operation is "
                        << ((m_value.is_lock_free()) ? "lock free" : "NOT lock free")
                        << std::endl;
        }


        //External synchronized interface
        virtual void updateProgress(int value) {
            m_value.store(value);
            notifyProgress();
        }


        //External interface
        virtual void show() const {
            m_progress->show();
        }


        //Internal interface
        virtual int getValue() const {
            return m_value.load();
        }

    };
}

#endif
