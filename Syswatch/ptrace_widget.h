#ifndef PTRACE_WIDGET_H
#define PTRACE_WIDGET_H

#include <QWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QTableWidget>
#include <QtCore/QTextCodec>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui>
#include <QTimer>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/user.h>

class Syscall
{
public:
    int id;
    QString name;
    int times;
};

class Ptrace_widget : public QWidget
{
    Q_OBJECT
public:
    explicit Ptrace_widget(QWidget *parent = 0);
private:
    QString syscallname[512];
    int syscall_id[512];
    Syscall syscall[512];
    int sys_count;

    QLabel * label;
    QLineEdit * edit;
    QPushButton * button;
    QTabWidget * tabwidget;
    QTableWidget * tableWidget;
    QGridLayout * layout;

    void get_syscall_name();
    void ptrace_path(char * path);
    void PaintTable();
    void tablechange();
    QString itoa(int m);
signals:

public slots:
    void begin_trace();
};

#endif // PTRACE_WIDGET_H
