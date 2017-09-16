#include <QtWidgets>
#include <iostream>
#include "graph_qt.h"

namespace gui_qt {

    namespace {
        const std::vector<QColor> colors = {
            Qt::black,
            Qt::darkGreen,
            Qt::red,
            Qt::blue,
            Qt::darkMagenta,
            Qt::darkCyan,
            Qt::yellow
        };

        QColor getColor(std::size_t idx) {
            return colors[idx % colors.size()];
        }
    }

    GraphQt::GraphQt(gui::GuiGraphInterfaceInt* ifc, QWidget* parent)
        : QWidget(parent), m_ifc(ifc) {
        setWindowTitle(tr("FANN test"));
        setStyleSheet("background-color:lightGray;");
        resize(1024, 512);

        connect(this, SIGNAL(signalNewData()), this, SLOT(slotNewData()));
    }

    void GraphQt::slotNewData() {
        m_ifc->newDataInit();
        update();
    }

    QPointF GraphQt::d2phy(const std::pair<std::size_t, double>& d) const {
        auto dataLen = m_ifc->graphDataLength();
        auto dataMax = m_ifc->graphDataMax();
        auto dataMin = m_ifc->graphDataMin();

        qreal dRX = szX / dataLen;
        qreal dRY = szY / (dataMax - dataMin);
        auto p = QPointF(dRX * d.first, szY - dRY * (d.second - dataMin));
        return p - sOff;
    }

    std::pair<std::size_t, double> GraphQt::phy2d(const QPointF& point) const {
        auto dataLen = m_ifc->graphDataLength();
        auto dataMax = m_ifc->graphDataMax();
        auto dataMin = m_ifc->graphDataMin();

        qreal dLX = static_cast<qreal>(dataLen) / szX;
        qreal dLY = static_cast<qreal>(dataMax - dataMin) / szY;
        auto p = -point + sOff;
        auto x = dLX * p.x();
        auto y = dLY * p.y() + dataMin;
        return std::make_pair(static_cast<std::size_t>(x), y);
    }

    void GraphQt::initPainter(QPainter& painter) const {
        painter.setRenderHint(QPainter::Antialiasing);
        painter.translate(width() / 2, height() / 2);
        painter.scale(width() / szX, height() / szY);
    }

    void GraphQt::drawGrid(QPainter& painter) const {
        painter.setPen(QPen(Qt::gray, 0));

        for(std::size_t j = 0; j < XCNT; ++j) {
            auto pointA = QPointF(j * XSIZE, 0) - sOff;
            auto pointB = QPointF(j * XSIZE, szY) - sOff;
            painter.drawLine(pointA, pointB);
        }

        for(std::size_t j = 0; j < YCNT; ++j) {
            auto pointA = QPointF(0, j * YSIZE) - sOff;
            auto pointB = QPointF(szX, j * YSIZE) - sOff;
            painter.drawLine(pointA, pointB);
        }
    }

    void GraphQt::drawAxis(QPainter& painter) const {
        auto dataLen = m_ifc->graphDataLength();
        auto dataMax = m_ifc->graphDataMax();
        auto dataMin = m_ifc->graphDataMin();

        if(dataMin < 0 && dataMax > 0) {
            painter.setPen(QPen(Qt::red, 0));
            auto pointA = d2phy(std::make_pair(0, 0.0));
            auto pointB = d2phy(std::make_pair(dataLen, 0.0));
            painter.drawLine(pointA, pointB);
        }
    }

    void GraphQt::drawScale(QPainter& painter) const {
        painter.setPen(Qt::darkRed);
        for(std::size_t j = 1; j < YCNT; ++j) {
            auto pointB = QPointF(szX, j * YSIZE) - sOff;
            auto v = phy2d(pointB);
            auto boundingRect = QRectF(pointB - QPointF(256, 12), QSizeF(256, 24));
            painter.setFont(QFont("Courier New", 14, QFont::Bold));
            auto s = QString::number(v.second, 'f', 2);
            painter.drawText(boundingRect, Qt::AlignRight | Qt::AlignVCenter, s);
        }
    }

    void GraphQt::drawInfo(QPainter& painter) const {
        auto& graphName = m_ifc->graphName();
        auto& mask = m_ifc->dataMask();
        auto& dataV = m_ifc->dataVector();

        painter.setFont(QFont("Courier New", 18, QFont::Bold));
        auto boundingRect = QRectF(QPointF(10, 10) - sOff, QSizeF(768, 32));
        painter.setPen(Qt::black);
        painter.drawText(boundingRect, Qt::AlignLeft, QString(graphName.c_str()));

        std::size_t idx = 1;
        for(const auto& m : mask) {
            painter.setPen(getColor(m));
            boundingRect = QRectF(QPointF(10, 10 + 32 * idx++) - sOff, QSizeF(512, 32));
            auto name = dataV.getNameAt(m);
            painter.drawText(boundingRect, Qt::AlignLeft, QString(name.c_str()));
        }
    }

    void GraphQt::drawGraph(QPainter& painter) const {
        auto& mask = m_ifc->dataMask();
        auto& dataV = m_ifc->dataVector();

        std::vector<QPointF> prev(dataV.elementDataSize());
        auto& elem = dataV.getElements();
        std::size_t idx = 0;
        for(const auto& x : elem) {
            for(const auto& m : mask) {
                auto point = d2phy(std::make_pair(idx, x.dataAt(m)));
                if(idx > 0) {
                    auto pen = QPen(getColor(m), 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                    painter.setPen(pen);
                    painter.drawLine(prev.at(m), point);
                }
                prev[m] = point;
            }
            ++idx;
        }
    }

    void GraphQt::paintEvent(QPaintEvent *) {
        QPainter painter(this);
        initPainter(painter);

        drawGrid(painter);
        drawAxis(painter);
        drawGraph(painter);
        drawScale(painter);
        drawInfo(painter);
    }
}
