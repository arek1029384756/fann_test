#include <QtWidgets>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "window.h"

// *** temporary version, hardcoded values to be replaced

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("FANN test"));
    resize(1024, 512);

    //QTimer::singleShot(100, this, SLOT(updateData()));
}

void Window::updateData()
{
    update();
    //QTimer::singleShot(1000, this, SLOT(updateData()));
}

double Window::getMaxStock(const std::vector<double>* const data) const
{
    auto itMax = std::max_element(data->begin(), data->end());
    return *itMax;
}

void Window::setData(const std::vector<double>* const data, const std::string& filename)
{
    m_fileName = filename;
    m_dataLen = data->size();
    m_dataMax = getMaxStock(data);
    m_data = data;

    std::cout << "File:\t\t" << m_fileName << std::endl;
    std::cout << "Data length:\t" << m_dataLen << std::endl;
    std::cout << "Data max:\t" << m_dataMax << std::endl;
}

QPointF Window::d2phy(const std::pair<std::size_t, double>& d) const
{
    static const auto soff = QPointF(1024.0, 512.0);
    qreal dRX = 2048.0 / m_dataLen;
    qreal dRY = 1024.0 / m_dataMax;
    return QPointF(dRX * d.first, 1024.0 - dRY * d.second) - soff;
}

void Window::initPainter(QPainter& painter) const
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(width() / 2048.0, height() / 1024.0);
}

void Window::drawGrid(QPainter& painter) const
{
    painter.setPen(QPen(Qt::gray, 0));
    painter.setBrush(Qt::lightGray);
    painter.drawRect(0 - 1024, 0 - 512, 2048, 1024);

    for(std::size_t j = 0; j < 32; ++j) {
        painter.drawLine(j * 64 - 1024, 0 - 512, j * 64 - 1024, 1024 - 512);
    }

    for(std::size_t j = 0; j < 16; ++j) {
        painter.drawLine(0 - 1024, j * 64 - 512, 2048 - 1024, j * 64 - 512);
    }
}

void Window::drawInfo(QPainter& painter) const
{
    auto boundingRect = QRectF(0 - 1024, 0 - 512, 512, 64);
    //painter.drawRect(boundingRect);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Courier New", 18, QFont::Bold));
    painter.drawText(boundingRect, Qt::AlignCenter, QString(m_fileName.c_str()));
}

void Window::drawGraph(QPainter& painter) const
{
    auto pen = QPen(Qt::blue, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    QPointF prev = QPointF(0, 0);
    std::size_t idx = 0;
    for(auto x : *m_data) {
        auto point = d2phy(std::make_pair(idx, x));
        if(idx > 0) {
            painter.drawLine(prev, point);
        }
        prev = point;
        ++idx;
    }
}

void Window::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    initPainter(painter);

    drawGrid(painter);
    drawGraph(painter);
    drawInfo(painter);
}
