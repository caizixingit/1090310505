#ifndef NETWIDGET_H
#define NETWIDGET_H

#include <QWidget>
#include <QColor>
#include <QHash>
#include <QWidget>


class netWidget : public QWidget
{
    Q_OBJECT
public:
    explicit netWidget(QWidget *parent = 0);

    void paint(QPainter &painter);
    void paintnet(QPainter &painter);
    QString Getunit(int m);
    int Findmax();
    void Getmax(double d);

    double net_rcv[120];
    double net_trmt[120];
    int net_ptr;
    int net_count;
    int max_unit;
    int unit;
signals:


protected:
    void paintEvent(QPaintEvent *event);

public slots:

};

#endif // NETWIDGET_H
