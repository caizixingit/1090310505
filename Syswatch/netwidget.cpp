#include "netwidget.h"

#include <QtGui>
#include <QPen>

netWidget::netWidget(QWidget *parent) :
    QWidget(parent)
{
    net_count = 0;
    net_ptr = 0;
    for(int i = 0; i < 120; i++)
    {
        net_trmt[i] = 0;
        net_rcv[i] = 0;
    }
}

void netWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(painter);
    painter.end();
}

void netWidget::paint(QPainter &painter)
{
    painter.setClipRect(QRect(0, 0, 600, 300));
    QPen pen(Qt::black,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter.setPen(pen);
    painter.fillRect(QRect(0, 0, 600, 300), Qt::white);
    for(int i = 0; i < 121; i++)
    {
        painter.drawPoint(70 + 4 * i, 10);
        painter.drawPoint(70 + 4 * i, 70);
        painter.drawPoint(70 + 4 * i, 130);
    }
    for(int i = 0; i < 21; i++)
    {
        painter.drawPoint(70, 10 + 6 * i);
        painter.drawPoint(150, 10 + 6 * i);
        painter.drawPoint(230, 10 + 6 * i);
        painter.drawPoint(310, 10 + 6 * i);
        painter.drawPoint(390, 10 + 6 * i);
        painter.drawPoint(470, 10 + 6 * i);
        painter.drawPoint(550, 10 + 6 * i);
    }
    QString unit = Getunit(Findmax());
    char ss[20];
    QString s[3];
    sprintf(ss,"%d",max_unit);
    s[0] = ss + unit;
    sprintf(ss,"%d",max_unit / 2);
    s[1] = ss + unit;
    s[2] = "0" + unit;

    painter.drawText(QRectF(5,5,60,40),s[0]);
    painter.drawText(QRectF(5,65,60,40),s[1]);
    painter.drawText(QRectF(5,125,60,40),s[2]);
    painter.drawText(QRectF(70,140,40,30),"0");
    painter.drawText(QRectF(145,140,40,30),"10");
    painter.drawText(QRectF(225,140,40,30),"20");
    painter.drawText(QRectF(305,140,40,30),"30");
    painter.drawText(QRectF(385,140,40,30),"40");
    painter.drawText(QRectF(465,140,40,30),"50");
    painter.drawText(QRectF(545,140,40,30),"60");
    paintnet(painter);
}
void netWidget::paintnet(QPainter &painter)
{
    QPen pen(Qt::green,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter.setPen(pen);
    int ptr = 0, rcv0 = 0, rcv1 = 0, trmt0 = 0, trmt1 = 0, max = 1;
    double x0 = 0.0, y0 = 0.0, x1 = 0.0, y1 = 0.0;
    max = max_unit;
    for(int j = 1; j < unit; j++)
        max = max * 1024;
    for(int i = 0; i < net_count; i++)
    {
        if(i >= 120)
            break;
        if(net_count > 120)
            ptr = net_ptr;
        if(i != 0 )
        {
           rcv0 = (net_rcv[( ptr + i - 1 ) % 120] * 100 ) / max;
           rcv1 = (net_rcv[( ptr + i ) % 120] * 100) / max;
           x0 = 70 + 4 * (i - 1);
           x1 = 70 + 4 * i;
           y0 = 130 - rcv0 * 1.2;
           y1 = 130 - rcv1 * 1.2;
           painter.drawLine(QPoint(x0,y0),QPoint(x1,y1));
        }
    }
    QPen pen1(Qt::blue,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter.setPen(pen1);
    for(int i = 0; i < net_count; i++)
    {
        if(i >= 120)
            break;
        if(net_count > 120)
            ptr = net_ptr;
        if(i != 0 )
        {
           trmt0 = (net_trmt[( ptr + i - 1 ) % 120] * 100) / max;
           trmt1 = (net_trmt[( ptr + i ) % 120] * 100 ) / max ;
           x0 = 70 + 4 * (i - 1);
           x1 = 70 + 4 * i;
           y0 = 125 - trmt0 * 1.2;
           y1 = 125 - trmt1 * 1.2;
           painter.drawLine(QPoint(x0,y0),QPoint(x1,y1));
        }
    }
}
int netWidget::Findmax()
{
    int rcvmax = net_rcv[0];
    int trmtmax = net_trmt[0];
    for(int i = 1; i < net_count; i++)
    {
        if(i >= 120)
            break;
        if(net_rcv[i] > rcvmax)
            rcvmax = net_rcv[i];
        if(net_trmt[i] > trmtmax)
            trmtmax = net_trmt[i];
    }
    if(rcvmax > trmtmax)
        return rcvmax;
    else
        return trmtmax;
}


QString netWidget::Getunit(int m)
{
    QString s;
    int n = m, i = 0;
    double d;
    char ss[20];
    while(n > 0)
    {
        n = n / 1024;
        i++;
    }
    switch(i)
    {
    case 0:
        s = "B/s";
        break;
    case 1:
        s = tr("B/s");
        break;
    case 2:
        d = (m * 2) / 1024;
        sprintf(ss,"%.2f",d);
        s = tr("KiB/s");
        break;
    case 3:
        d = (m * 2) / 1024 / 1024;
        sprintf(ss,"%.2f",d);
        s = tr("MiB/s");
        break;
    case 4:
        d = (m * 2) / 1024 / 1024 / 1024;
        sprintf(ss,"%.2f",d);
        s = tr("GiB/s");
        break;
    default:
        printf("error\n");
        break;
    }
    unit = i;
    Getmax(d);
    return s;
}

void netWidget::Getmax(double d)
{
    int i;
    for(i = 10; i < 1024; i = i * 2)
    {
        if(d <= i)
            break;
    }
    max_unit = i;
}
