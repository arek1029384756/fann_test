#include <QtWidgets>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <chrono>
#include <fnn_math.hpp>
#include "window.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("FANN test"));
    setStyleSheet("background-color:lightGray;");
    resize(1024, 512);

    //QTimer::singleShot(100, this, SLOT(updateData()));
}

void Window::updateData()
{
    update();
    //QTimer::singleShot(1000, this, SLOT(updateData()));
}

void Window::getMinMaxStock(const mw::DataVector* const dataV, double& min, double& max) const
{
    auto res = math::limits(*dataV, m_mask);
    min = res.first;
    max = res.second;
}

void Window::setData(const mw::DataVector* const data, const std::set<int>& mask, const std::string& filename)
{
    m_fileName = filename;
    m_mask = mask;
    m_data = data;
    auto elem = m_data->getElements();
    m_dataLen = elem.size();
    getMinMaxStock(m_data, m_dataMin, m_dataMax);

    std::cout << "File:\t\t" << m_fileName << std::endl;
    std::cout << "Data length:\t" << m_dataLen << std::endl;
    std::cout << "Data max:\t" << m_dataMax << std::endl;
    std::cout << "Data min:\t" << m_dataMin << std::endl << std::endl;
}

QPointF Window::d2phy(const std::pair<std::size_t, double>& d) const
{
    qreal dRX = szX / m_dataLen;
    qreal dRY = szY / (m_dataMax - m_dataMin);
    auto p = QPointF(dRX * d.first, szY - dRY * (d.second - m_dataMin));
    return p - sOff;
}

std::pair<std::size_t, double> Window::phy2d(const QPointF& point) const
{
    qreal dLX = static_cast<qreal>(m_dataLen) / szX;
    qreal dLY = static_cast<qreal>(m_dataMax - m_dataMin) / szY;
    auto p = -point + sOff;
    auto x = dLX * p.x();
    auto y = dLY * p.y() + m_dataMin;
    return std::make_pair(static_cast<std::size_t>(x), y);
}

void Window::initPainter(QPainter& painter) const
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(width() / szX, height() / szY);
}

void Window::drawGrid(QPainter& painter) const
{
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

void Window::drawAxis(QPainter& painter) const
{
    if(m_dataMin < 0 && m_dataMax > 0) {
        painter.setPen(QPen(Qt::red, 0));
        auto pointA = d2phy(std::make_pair(0, 0.0));
        auto pointB = d2phy(std::make_pair(m_dataLen, 0.0));
        painter.drawLine(pointA, pointB);
    }
}

void Window::drawScale(QPainter& painter) const
{
    painter.setPen(Qt::darkRed);
    for(std::size_t j = 1; j < YCNT; ++j) {
        auto pointB = QPointF(szX, j * YSIZE) - sOff;
        auto v = phy2d(pointB);
        auto boundingRect = QRectF(pointB - QPointF(128, 12), QSizeF(128, 24));
        painter.setFont(QFont("Courier New", 14, QFont::Bold));
        auto s = QString::number(v.second, 'f', 2);
        painter.drawText(boundingRect, Qt::AlignRight | Qt::AlignVCenter, s);
    }
}

void Window::drawInfo(QPainter& painter) const
{
    auto boundingRect = QRectF(QPointF(0, 0) - sOff, QSizeF(512, 64));
    painter.setPen(Qt::black);
    painter.setFont(QFont("Courier New", 18, QFont::Bold));
    painter.drawText(boundingRect, Qt::AlignCenter, QString(m_fileName.c_str()));
}

void Window::drawGraph(QPainter& painter) const
{
    static const QColor colors[] = { Qt::yellow,
        Qt::green,
        Qt::red,
        Qt::blue,
        Qt::darkMagenta,
        Qt::darkCyan,
        Qt::black };


    auto elem = m_data->getElements();
    std::vector<QPointF> prev(elem[0].dataSize());
    std::size_t idx = 0;
    for(const auto& x : elem) {
        auto v = x.getData();
        for(const auto& m : m_mask) {
            auto point = d2phy(std::make_pair(idx, v[m]));
            if(idx > 0) {
                auto pen = QPen(colors[m], 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                painter.setPen(pen);
                painter.drawLine(prev[m], point);
            }
            prev[m] = point;
        }
        ++idx;
    }
}

void Window::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    initPainter(painter);

    drawGrid(painter);
    drawAxis(painter);
    drawGraph(painter);
    drawScale(painter);
    drawInfo(painter);
}
