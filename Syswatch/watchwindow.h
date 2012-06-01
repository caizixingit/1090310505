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
<<<<<<< HEAD
#include "displaywidget.h"
#include "netwidget.h"
=======
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9

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
<<<<<<< HEAD

    QGridLayout * mLayout;
    QVBoxLayout * vLayout;

    QPushButton * endbutton;
    QTableWidget * tableWidget;
    QHBoxLayout * burdenLayout;

    QLabel *label_cpu;
=======
    QGridLayout * mLayout;
    QVBoxLayout * vLayout;
    QPushButton * endbutton;
    QTableWidget * tableWidget;

   // QTableWidgetItem items[1000][5];

>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label12;
    QLabel *label22;
    QLabel *label32;
    QLabel *label42;

<<<<<<< HEAD
    QLabel *label_net;
    QLabel *label_rcv;
    QLabel *label_rcvp;
    QLabel *label_trmt;
    QLabel *label_trmtp;
    QLabel *label_rcv1;
    QLabel *label_rcvp1;
    QLabel *label_trmt1;
    QLabel *label_trmtp1;

    QLabel * label_burden1;
    QLabel * label_burden2;

=======
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9

    void Getpinfo_first();
    void Getpinfo();
    void Getminfo();
    void Getcpuinfo();
    void PaintTable();
    void PaintMem();
    void tablechange();
<<<<<<< HEAD
    void Getburden();
    QString Getunit(int m);
    QString itoa(int m);

    displayWidget * dwidget;
    netWidget * nwidget;

=======
    QString itoa(int m);

>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
signals:

public slots:
    void endproc();
    void ontimerout();
    void cputimerout();
    void tabchange();
};

#endif // WATCHWINDOW_H
