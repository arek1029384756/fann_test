#ifndef PROGRESSQT_H
#define PROGRESSQT_H

#include <QDialog>
#include <QFormLayout>
#include <QProgressBar>
#include <memory>
#include <gui_progress_interface.hpp>

namespace gui_qt {

    class ProgressQt : public QDialog {
        Q_OBJECT

        gui::GuiProgressInterfaceInt* m_ifc;

        std::shared_ptr<QFormLayout> m_layout;

        std::shared_ptr<QProgressBar> m_progressA;

        std::shared_ptr<QProgressBar> m_progressB;

        public slots:
        void slotNewData();

        signals:
        void signalNewData();

        public:
        ProgressQt(gui::GuiProgressInterfaceInt* ifc, QDialog* parent = 0);
    };
}

#endif
