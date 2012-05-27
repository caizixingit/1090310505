#ifndef WATCHWINDOW_H
#define WATCHWINDOW_H

#include <QDialog>
#include <QtGui/QTabWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QTableWidget>
#include <QtCore/QTextCodec>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui>
#include <QTimer>
#include <QTime>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "proc.h"

class Watchwindow : public QDialog
{
    Q_OBJECT
public:
    explicit Watchwindow(QWidget *parent = 0);

    Proc p;
    int itemcount;

    QTabWidget * tabwidget;
    QWidget * pwidget;
    QWidget * mwidget;
    QGridLayout * mLayout;
    QVBoxLayout * vLayout;
    QPushButton * endbutton;
    QTableWidget * tableWidget;

   // QTableWidgetItem items[1000][5];

    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label12;
    QLabel *label22;
    QLabel *label32;
    QLabel *label42;


    void Getpinfo_first();
    void Getpinfo();
    void Getminfo();
    void Getcpuinfo();
    void PaintTable();
    void PaintMem();
    void tablechange();
    QString itoa(int m);

signals:

public slots:
    void endproc();
    void ontimerout();
    void cputimerout();
    void tabchange();
};

#endif // WATCHWINDOW_H
