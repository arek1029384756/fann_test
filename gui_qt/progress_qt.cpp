#include <QtWidgets>
#include <iostream>
#include "progress_qt.h"

namespace gui_qt {

    ProgressQt::ProgressQt(gui::GuiProgressInterfaceInt* ifc, QDialog* parent)
        : QDialog(parent), m_ifc(ifc) {

        setWindowTitle(tr("Processing progress"));

        m_layout.reset(new QFormLayout());
        m_progressA.reset(new QProgressBar());
        m_progressB.reset(new QProgressBar());

        m_progressA->setMinimumWidth(400);
        m_progressB->setMinimumWidth(400);

        m_layout->addRow(tr("Progress A"), m_progressA.get());
        m_layout->addRow(tr("Progress B"), m_progressB.get());

        m_layout->setSizeConstraint(QLayout::SetFixedSize);

        setLayout(m_layout.get());

        m_progressA->setMinimum(0);
        m_progressA->setMaximum(100);
        m_progressA->setValue(74);

        m_progressB->setMinimum(0);
        m_progressB->setMaximum(100);
        m_progressB->setValue(12);

    }

}
