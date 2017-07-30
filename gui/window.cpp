#include <QtWidgets>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <chrono>
#include <fnn_math.hpp>
#include "window.h"

namespace {
    static const QColor colors[] = { Qt::yellow,
        Qt::darkGreen,
        Qt::red,
        Qt::blue,
        Qt::darkMagenta,
        Qt::darkCyan,
        Qt::black };
}

Window::Window(QWidget *parent)
    : QWidget(parent), m_dataV(0)
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

void Window::getMinMaxStock(double& min, double& max) const
{
    auto res = math::compute<math::Limits>(*m_dataV, m_mask);
    min = res.first;
    max = res.second;
}

void Window::testDataPresence() const {
    try {
        std::cout << "Test of data presence on vector 0" << std::endl;
        auto names = m_dataV->getNames();
        auto elem = m_dataV->getElements();
        for(const auto& m : m_mask) {
            auto data = elem.at(0).getData();
            auto name = names.at(m);
            auto val = data.at(m);
            std::cout << "#" << m << ": " << name << "  " << val << std::endl;
        }
        std::cout << std::endl;
    } catch(const std::out_of_range& e) {
        throw std::runtime_error(
                std::string("[2] Chosen dataset index exceeds range. ") +
                std::string("Maybe your input file lacks some columns? '") +
                e.what() + std::string("'"));
    }
}

void Window::setData(const mw::DataVector* const dataV, const std::set<int>& mask, const std::string& filename)
{
    m_fileName = filename;
    m_mask = mask;
    m_dataV = dataV;
    auto elem = m_dataV->getElements();
    m_dataLen = elem.size();

    testDataPresence();
    getMinMaxStock(m_dataMin, m_dataMax);

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
    painter.setFont(QFont("Courier New", 18, QFont::Bold));
    auto boundingRect = QRectF(QPointF(10, 10) - sOff, QSizeF(512, 32));
    painter.setPen(Qt::black);
    painter.drawText(boundingRect, Qt::AlignLeft, QString(m_fileName.c_str()));

    std::size_t idx = 1;
    auto names = m_dataV->getNames();
    for(const auto& m : m_mask) {
        painter.setPen(colors[m]);
        boundingRect = QRectF(QPointF(10, 10 + 32 * idx++) - sOff, QSizeF(512, 32));
        painter.drawText(boundingRect, Qt::AlignLeft, QString(names.at(m).c_str()));
    }
}

void Window::drawGraph(QPainter& painter) const
{
    std::vector<QPointF> prev(m_dataV->elementDataSize());
    auto elem = m_dataV->getElements();
    std::size_t idx = 0;
    for(const auto& x : elem) {
        auto v = x.getData();
        for(const auto& m : m_mask) {
            auto point = d2phy(std::make_pair(idx, v.at(m)));
            if(idx > 0) {
                auto pen = QPen(colors[m], 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                painter.setPen(pen);
                painter.drawLine(prev.at(m), point);
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
