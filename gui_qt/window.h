#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <vector>
#include <memory>
#include <set>
#include <data_format.hpp>
#include <gui_interface.hpp>
//#include <exception>
//#include <stdexcept>    //covering g++ version differences

namespace gui {

    class Window : public QWidget {
        Q_OBJECT

        static constexpr double szX = 2048.0;

        static constexpr double szY = 1024.0;

        static constexpr auto sOff = QPointF(szX / 2, szY / 2);

        static constexpr int XCNT = 32;

        static constexpr int YCNT = 16;

        static constexpr auto XSIZE = szX / XCNT;

        static constexpr auto YSIZE = szY / YCNT;


        GuiGraphInterfaceInt* m_ifc;

        std::string m_graphName;

        std::size_t m_dataLen;

        double m_dataMax;

        double m_dataMin;

        const mw::DataVector* m_dataV;

        std::set<int> m_mask;

        void getMinMaxStock(double& min, double& max) const;

        void testDataPresence() const;

        QPointF d2phy(const std::pair<std::size_t, double>& d) const;

        std::pair<std::size_t, double> phy2d(const QPointF& point) const;

        void initPainter(QPainter& painter) const;

        void drawGrid(QPainter& painter) const;

        void drawAxis(QPainter& painter) const;

        void drawInfo(QPainter& painter) const;

        void drawScale(QPainter& painter) const;

        void drawGraph(QPainter& painter) const;

        public slots:
        void updateData();

        public:
        Window(GuiGraphInterfaceInt* ifc, QWidget* parent = 0);
        void setData(const mw::DataVector* const dataV, const std::set<int>& mask, const std::string& graphName);

        protected:
        void paintEvent(QPaintEvent *event) override;
    };
}

#endif
