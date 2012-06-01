#include "displaywidget.h"
#include <QtGui>
#include <QPen>

displayWidget::displayWidget(QWidget *parent) :
    QWidget(parent)
{
    cpu_use_count = 0;
    cpu_use_ptr = 0;
    for(int i = 0; i < 120; i++)
        cpu_use[i] = 0;
}

void displayWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(painter);
    painter.end();
}

void displayWidget::paint(QPainter &painter)
{
    painter.setClipRect(QRect(0, 0, 600, 300));
    QPen pen(Qt::black,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter.setPen(pen);
    painter.fillRect(QRect(0, 0, 600, 300), Qt::white);
    for(int i = 0; i < 121; i++)
    {
        painter.drawPoint(50 + 4 * i, 10);
        painter.drawPoint(50 + 4 * i, 70);
        painter.drawPoint(50 + 4 * i, 130);
    }
    for(int i = 0; i < 21; i++)
    {
        painter.drawPoint(50, 10 + 6 * i);
        painter.drawPoint(130, 10 + 6 * i);
        painter.drawPoint(210, 10 + 6 * i);
        painter.drawPoint(290, 10 + 6 * i);
        painter.drawPoint(370, 10 + 6 * i);
        painter.drawPoint(450, 10 + 6 * i);
        painter.drawPoint(530, 10 + 6 * i);
    }
    painter.drawText(QRectF(5,5,40,30),"100%");
    painter.drawText(QRectF(5,65,40,30),"50%");
    painter.drawText(QRectF(5,125,40,30),"0%");
    painter.drawText(QRectF(50,140,40,30),"0");
    painter.drawText(QRectF(125,140,40,30),"10");
    painter.drawText(QRectF(205,140,40,30),"20");
    painter.drawText(QRectF(285,140,40,30),"30");
    painter.drawText(QRectF(365,140,40,30),"40");
    painter.drawText(QRectF(445,140,40,30),"50");
    painter.drawText(QRectF(525,140,40,30),"60");
    paintcpu(painter);

}
void displayWidget::paintcpu(QPainter &painter)
{
    QPen pen(Qt::red,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter.setPen(pen);
    int ptr = 0, cpu0 = 0, cpu1 = 0;
    double x0 = 0.0, y0 = 0.0, x1 = 0.0, y1 = 0.0;
    for(int i = 0; i < cpu_use_count; i++)
    {
        if(i >= 120)
            break;
        if(cpu_use_count > 120)
            ptr = cpu_use_ptr;
        if(i != 0 )
        {
           cpu0 = cpu_use[( ptr + i - 1 ) % 120];
           cpu1 = cpu_use[( ptr + i ) % 120];
           x0 = 50 + 4 * (i - 1);
           x1 = 50 + 4 * i;
           y0 = 125 - cpu0 * 1.2;
           y1 = 125 - cpu1 * 1.2;
           painter.drawLine(QPoint(x0,y0),QPoint(x1,y1));
        }

    }
}
