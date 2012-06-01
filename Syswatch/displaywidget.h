#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include <QColor>
#include <QHash>
#include <QWidget>

class displayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit displayWidget(QWidget *parent = 0);

    void paint(QPainter &painter);
    void paintcpu(QPainter &painter);

    int cpu_use[120];
    int cpu_use_ptr;
    int cpu_use_count;
signals:


protected:
    void paintEvent(QPaintEvent *event);
public slots:

};

#endif // DISPLAYWIDGET_H
