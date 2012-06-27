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
#include "displaywidget.h"
#include "netwidget.h"
#include "ptrace_widget.h"

class Watchwindow : public QDialog
{
    Q_OBJECT
public:
    explicit Watchwindow(QWidget *parent = 0);

    Proc p;
    int itemcount;

    QTabWidget * tabwidget;
    QWidget * bwidget;
    QWidget * pwidget;
    QWidget * mwidget;
    QWidget * dfwidget;

    QGridLayout * mLayout;
    QVBoxLayout * bLayout;
    QVBoxLayout * vLayout;

    QPushButton * endbutton;

    QTableWidget * tableWidget;
    QHBoxLayout * burdenLayout;

    QTableWidget * dfWidget;
    QVBoxLayout * dfLayout;

    QLabel *label_cpu;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label12;
    QLabel *label22;
    QLabel *label32;
    QLabel *label42;

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

    QLabel * label_df;

    void Getpinfo_first();
    void Getpinfo();
    void Getminfo();
    void Getcpuinfo();
    void PaintTable();
    void PaintMem();
    void PaintDf();
    void PaintBasic();
    void tablechange();
    void Getburden();
    void Dftablechange();
    QString Getunit(int m);
    QString itoa(int m);

    displayWidget * dwidget;
    netWidget * nwidget;
    Ptrace_widget * twidget;


signals:

public slots:
    void endproc();
    void ontimerout();
    void cputimerout();
    void dftimerout();
    void tracep(int x, int y);
};

#endif // WATCHWINDOW_H
