#include "watchwindow.h"
#include "proc.h"

Watchwindow::Watchwindow(QWidget *parent) :
    QDialog(parent)
{
    p.init();
    Getminfo();
    Getpinfo_first();
    QTimer * timer = new QTimer(this);
    timer->start(5000);
    connect(timer, SIGNAL(timeout()), this, SLOT(ontimerout()));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    tabwidget = new QTabWidget;
    pwidget = new QWidget;
    mwidget = new QWidget;
    vLayout = new QVBoxLayout();
    mLayout = new QGridLayout();

    endbutton = new QPushButton(tr("结束进程"));
    connect(endbutton, SIGNAL(clicked()), this, SLOT(endproc()));

    PaintTable();

    PaintMem();


    tabwidget->addTab(pwidget,tr("进程"));
    tabwidget->addTab(mwidget,tr("资源"));
    //connect(tabwidget,SIGNAL(currentChanged()),this,SLOT(tabchange()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(tabwidget,1,1,100,160);
    setLayout(mainLayout);

}

void Watchwindow::tabchange()
{
    int m = tabwidget->currentIndex();
    if(m == 1)
    {
        Getminfo();
        Getpinfo();
        PaintTable();
    }
    else if(m == 2)
    {
        Getminfo();
        Getcpuinfo();
        PaintMem();
    }
}

void Watchwindow::PaintMem()
{
    QTimer * timers = new QTimer(this);
    timers->start(10000);
    connect(timers, SIGNAL(timeout()), this, SLOT(cputimerout()));

    Getcpuinfo();
    int m = (p.mem_stat.total - p.mem_stat.free ) * 100 / p.mem_stat.total;
    QString s = itoa(m) + QLatin1String("%");
    QString s1 = "%";

    label1 = new QLabel(tr("内存总量："));
    label2 = new QLabel(tr("空闲内存："));
    label3 = new QLabel(tr("内存使用率："));
    label4 = new QLabel(tr("cpu使用率："));
    label12 = new QLabel(itoa(p.mem_stat.total));
    label22 = new QLabel(itoa(p.mem_stat.free));
    label32 = new QLabel(s);
    label42 = new QLabel(s1);

    mLayout->addWidget(label1,1,1,1,1);
    mLayout->addWidget(label12,1,3,2,1);
    mLayout->addWidget(label2,3,1,2,1);
    mLayout->addWidget(label22,3,3,2,1);
    mLayout->addWidget(label3,5,1,2,1);
    mLayout->addWidget(label32,5,3,2,1);
    mLayout->addWidget(label4,7,1,2,1);
    mLayout->addWidget(label42,7,3,2,1);

    mwidget->setLayout(mLayout);
}

void Watchwindow::PaintTable()
{

    tableWidget = new QTableWidget(p.count,5);
    tableWidget->setWindowTitle("QTableWidget & Item");
    tableWidget->resize(550, 500); //设置表格
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    //tableWidget->setEditTriggers(QAbstractItemView::CurrentChanged);  //不可更改
    QStringList header;
    header<<tr("进程名")<<tr("进程ID")<<tr("状态")<<tr("内存")<<tr("内存%%");
    tableWidget->setHorizontalHeaderLabels(header);
    tablechange();
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不可更改
    vLayout->addWidget(tableWidget);
    vLayout->addWidget(endbutton);
    pwidget->setLayout(vLayout);

}

void Watchwindow::tablechange()
{
    int m = 0;
    tableWidget->setRowCount(p.count);
    for(int i = 0; i < p.count; i++)
    {
        QString s[5];
        s[0] = p.pinfo[i].name;
        s[1] = itoa(p.pinfo[i].pid);
        s[2] = p.pinfo[i].state;
        s[3] = itoa(p.pinfo[i].mem);
        s[4] = itoa((p.pinfo[i].mem * 10000 ) / p.mem_stat.total);

        tableWidget->setItem(m,0,new QTableWidgetItem(s[0]));
        tableWidget->setItem(m,1,new QTableWidgetItem(s[1]));
        tableWidget->setItem(m,2,new QTableWidgetItem(s[2]));
        tableWidget->setItem(m,3,new QTableWidgetItem(s[3]));
        tableWidget->setItem(m,4,new QTableWidgetItem(s[4]));
        m++;
    }
}



void Watchwindow::ontimerout()
{

    //for(int i = 0; i < p.count; i++)
      //  tableWidget->removeRow(i);

    Getpinfo();
    tablechange();
   // PaintTable();
}

void Watchwindow::cputimerout()
{
    Getminfo();
    Getcpuinfo();
    int m = (p.mem_stat.total - p.mem_stat.free ) * 100 / p.mem_stat.total;
    int ptr = ( p.cpu_ptr + 1 ) % 2;
    int m1 = p.cal_cpuoccupy ((CPU_OCCUPY *)&p.cpu_stat[p.cpu_ptr], (CPU_OCCUPY *)&p.cpu_stat[ptr]);
    QString s = itoa(m) + "%";
    QString s1 = itoa(m1) + "%";
    label12->setText(itoa(p.mem_stat.total));
    label22->setText(itoa(p.mem_stat.free));
    label32->setText(s);
    label42->setText(s1);
}

void Watchwindow::endproc()
{
    int m = tableWidget->currentRow();
    int n = p.pinfo[m].pid;
    kill(n , 15);

    tableWidget->removeRow(m);
}

QString Watchwindow::itoa(int m)
{
    int digit = 0;
    int n = m;
    QString str;
    while(n > 0)
    {
            n = n / 10;
            digit++;
    }
    while(m > 0)
    {
            str[digit - 1] =  m % 10 + '0';
            m = m / 10;
            digit--;
    }
    return str;
}

void Watchwindow::Getcpuinfo()
{

    //获取cpu使用情况
    p.get_cpuoccupy((CPU_OCCUPY *)&p.cpu_stat[p.cpu_ptr]);
    p.cpu_ptr = ( p.cpu_ptr + 1 ) % 2;

}

void Watchwindow::Getminfo()
{
    p.get_memoccupy ((MEM_OCCUPY *)&p.mem_stat);
    printf("%s  %u \n",p.mem_stat.name, p.mem_stat.total);
    printf("%s  %u \n",p.mem_stat.name2,p.mem_stat.free);
    printf("Memory : %u%% \n",(p.mem_stat.total - p.mem_stat.free ) * 100 / p.mem_stat.total);
}

void Watchwindow::Getpinfo()
{
    int t = p.pinfo[p.count - 1].pid;
    p.count = 0;
    for(int i = 1; i < t + 1000; i++)
    {
        p.pinfo[i].Tcount = 0;
        p.getprocinfo(i);
    }

}

void Watchwindow::Getpinfo_first()
{   
    int i = 0;
    for(i = 0; i < 10000; i++)
        p.pinfo[i].Tcount = 0;
    for(i = 1; i < 10000; i++)
        p.getprocinfo(i);
}




