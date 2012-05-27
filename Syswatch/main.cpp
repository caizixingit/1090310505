#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtCore/QTextCodec>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>


#include "watchwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Watchwindow window;
    window.show();
    return a.exec();
}
