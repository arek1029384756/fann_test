#ifndef GRAPHQT_H
#define GRAPHQT_H

#include <QWidget>
#include <gui_graph_interface.hpp>
//#include <exception>
//#include <stdexcept>    //covering g++ version differences

namespace gui_qt {

    class GraphQt : public QWidget {
        Q_OBJECT

        static constexpr double szX = 2048.0;

        static constexpr double szY = 1024.0;

        static constexpr auto sOff = QPointF(szX / 2, szY / 2);

        static constexpr int XCNT = 32;

        static constexpr int YCNT = 16;

        static constexpr auto XSIZE = szX / XCNT;

        static constexpr auto YSIZE = szY / YCNT;


        gui::GuiGraphInterfaceInt* m_ifc;

        QPointF d2phy(const std::pair<std::size_t, double>& d) const;

        std::pair<std::size_t, double> phy2d(const QPointF& point) const;

        void initPainter(QPainter& painter) const;

        void drawGrid(QPainter& painter) const;

        void drawAxis(QPainter& painter) const;

        void drawInfo(QPainter& painter) const;

        void drawScale(QPainter& painter) const;

        void drawGraph(QPainter& painter) const;

        public slots:
        void slotNewData();

        signals:
        void signalNewData();

        public:
        GraphQt(gui::GuiGraphInterfaceInt* ifc, QWidget* parent = 0);

        protected:
        void paintEvent(QPaintEvent *event) override;
    };
}

#endif
