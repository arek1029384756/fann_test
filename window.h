#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <vector>
#include <memory>
//#include <exception>
//#include <stdexcept>    //covering g++ version differences

class Window : public QWidget
{
    Q_OBJECT

    std::string m_fileName; 

    std::size_t m_dataLen;

    double m_dataMax;

    const std::vector<double>* m_data;

    double getMaxStock(const std::vector<double>* const data) const;

    QPointF d2phy(const std::pair<std::size_t, double>& d) const;

    void initPainter(QPainter& painter) const;

    void drawGrid(QPainter& painter) const;

    void drawInfo(QPainter& painter) const;

    void drawGraph(QPainter& painter) const;

    public slots:
        void updateData();

    public:
        Window(QWidget *parent = 0);
        void setData(const std::vector<double>* const data, const std::string& filename);

    protected:
        void paintEvent(QPaintEvent *event) override;
};

#endif
