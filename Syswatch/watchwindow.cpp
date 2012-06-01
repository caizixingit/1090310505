#include "watchwindow.h"
#include "proc.h"

Watchwindow::Watchwindow(QWidget *parent) :
    QDialog(parent)
{
    p.init();
    Getminfo();
    Getpinfo_first();
    QTimer * timer = new QTimer(this);
<<<<<<< HEAD
    timer->start(3000);
    connect(timer, SIGNAL(timeout()), this, SLOT(ontimerout()));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    dwidget = new displayWidget();
    nwidget = new netWidget();
    tabwidget = new QTabWidget;
    pwidget = new QWidget;
    vLayout = new QVBoxLayout();
    burdenLayout = new QHBoxLayout();

    mwidget = new QWidget;
=======
    timer->start(5000);
    connect(timer, SIGNAL(timeout()), this, SLOT(ontimerout()));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    tabwidget = new QTabWidget;
    pwidget = new QWidget;
    mwidget = new QWidget;
    vLayout = new QVBoxLayout();
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
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
<<<<<<< HEAD
    mainLayout->addWidget(tabwidget,1,1,100,100);
=======
    mainLayout->addWidget(tabwidget,1,1,100,160);
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
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

<<<<<<< HEAD

void Watchwindow::PaintMem()
{
    QTimer * timers = new QTimer(this);
    timers->start(500);
    connect(timers, SIGNAL(timeout()), this, SLOT(cputimerout()));

    Getcpuinfo();
    p.get_netdev();
=======
void Watchwindow::PaintMem()
{
    QTimer * timers = new QTimer(this);
    timers->start(10000);
    connect(timers, SIGNAL(timeout()), this, SLOT(cputimerout()));

    Getcpuinfo();
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
    int m = (p.mem_stat.total - p.mem_stat.free ) * 100 / p.mem_stat.total;
    QString s = itoa(m) + QLatin1String("%");
    QString s1 = "%";

<<<<<<< HEAD
    label_cpu = new QLabel(tr("CPU历史"));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::black);
    label_cpu->setPalette(pe);
=======
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
    label1 = new QLabel(tr("内存总量："));
    label2 = new QLabel(tr("空闲内存："));
    label3 = new QLabel(tr("内存使用率："));
    label4 = new QLabel(tr("cpu使用率："));
    label12 = new QLabel(itoa(p.mem_stat.total));
    label22 = new QLabel(itoa(p.mem_stat.free));
    label32 = new QLabel(s);
    label42 = new QLabel(s1);

<<<<<<< HEAD
    label_net = new QLabel(tr("网络历史:"));
    label_net->setPalette(pe);
    label_rcv = new QLabel(tr("总计已接收"));
    label_rcvp = new QLabel(tr("正在接收(绿)"));
    label_trmt = new QLabel(tr("总计已发送"));
    label_trmtp = new QLabel(tr("正在发送(蓝)"));
    label_rcv1 = new QLabel(tr(" "));
    label_rcvp1 = new QLabel(tr(" "));
    label_trmt1 = new QLabel(tr(" "));
    label_trmtp1 = new QLabel(tr(" "));


    mLayout->addWidget(label_cpu,0,0);
    mLayout->addWidget(label1,1,0);
    mLayout->addWidget(label12,1,1);
    mLayout->addWidget(label2,1,2);
    mLayout->addWidget(label22,1,3);
    mLayout->addWidget(label3,2,0);
    mLayout->addWidget(label32,2,1);
    mLayout->addWidget(label4,2,2);
    mLayout->addWidget(label42,2,3);

    mLayout->addWidget(dwidget,3,0,12,6);

    mLayout->addWidget(label_net,17,0);
    mLayout->addWidget(label_rcv,18,0);
    mLayout->addWidget(label_rcv1,18,1);
    mLayout->addWidget(label_rcvp,18,2);
    mLayout->addWidget(label_rcvp1,18,3);
    mLayout->addWidget(label_trmt,19,0);
    mLayout->addWidget(label_trmt1,19,1);
    mLayout->addWidget(label_trmtp,19,2);
    mLayout->addWidget(label_trmtp1,19,3);

    mLayout->addWidget(nwidget,20,0,8,6);

    mLayout->setRowMinimumHeight(20,150);
=======
    mLayout->addWidget(label1,1,1,1,1);
    mLayout->addWidget(label12,1,3,2,1);
    mLayout->addWidget(label2,3,1,2,1);
    mLayout->addWidget(label22,3,3,2,1);
    mLayout->addWidget(label3,5,1,2,1);
    mLayout->addWidget(label32,5,3,2,1);
    mLayout->addWidget(label4,7,1,2,1);
    mLayout->addWidget(label42,7,3,2,1);
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9

    mwidget->setLayout(mLayout);
}

void Watchwindow::PaintTable()
{
<<<<<<< HEAD
    Getburden();
=======
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9

    tableWidget = new QTableWidget(p.count,5);
    tableWidget->setWindowTitle("QTableWidget & Item");
    tableWidget->resize(550, 500); //设置表格
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    //tableWidget->setEditTriggers(QAbstractItemView::CurrentChanged);  //不可更改
    QStringList header;
    header<<tr("进程名")<<tr("进程ID")<<tr("状态")<<tr("内存")<<tr("内存%%");
    tableWidget->setHorizontalHeaderLabels(header);
    tablechange();
<<<<<<< HEAD
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可更改
    vLayout->addLayout(burdenLayout);
=======
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不可更改
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
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

<<<<<<< HEAD
void Watchwindow::Getburden()
{
    label_burden1 = new QLabel(tr("前一分钟、五分钟、十五分钟的平均负载："));
    p.get_burden();
    QString str;
    str = p.burden1;
    str.append(",     ");
    str.append(p.burden5);
    str.append(",     ");
    str.append(p.burden15);
    label_burden2 = new QLabel(str);
    burdenLayout->addWidget(label_burden1);
    burdenLayout->addWidget(label_burden2);
}
=======
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9


void Watchwindow::ontimerout()
{

    //for(int i = 0; i < p.count; i++)
      //  tableWidget->removeRow(i);

    Getpinfo();
    tablechange();
   // PaintTable();
}

<<<<<<< HEAD

=======
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
void Watchwindow::cputimerout()
{
    Getminfo();
    Getcpuinfo();
    int m = (p.mem_stat.total - p.mem_stat.free ) * 100 / p.mem_stat.total;
    int ptr = ( p.cpu_ptr + 1 ) % 2;
    int m1 = p.cal_cpuoccupy ((CPU_OCCUPY *)&p.cpu_stat[p.cpu_ptr], (CPU_OCCUPY *)&p.cpu_stat[ptr]);
<<<<<<< HEAD
    dwidget->cpu_use[dwidget->cpu_use_ptr] = m1;
    dwidget->cpu_use_ptr = (dwidget->cpu_use_ptr + 1) % 120;
    dwidget->cpu_use_count++;
=======
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
    QString s = itoa(m) + "%";
    QString s1 = itoa(m1) + "%";
    label12->setText(itoa(p.mem_stat.total));
    label22->setText(itoa(p.mem_stat.free));
    label32->setText(s);
    label42->setText(s1);
<<<<<<< HEAD

    p.get_netdev();
    int rcvp = 0, trmtp = 0;
    p.cal_netdev(&rcvp, &trmtp);
    nwidget->net_rcv[nwidget->net_ptr] = rcvp;
    nwidget->net_trmt[nwidget->net_ptr] = trmtp;
    nwidget->net_ptr = ( nwidget->net_ptr + 1) % 120;
    nwidget->net_count ++;
    label_rcv1->setText( Getunit(p.netdev_rcv).remove("/s") );
    label_trmt1->setText( Getunit(p.netdev_trmt).remove(("/s")) );
    label_rcvp1->setText( Getunit(rcvp));
    label_trmtp1->setText( Getunit(trmtp));
}

QString Watchwindow::Getunit(int m)
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
        s = "0 B/s";
        break;
    case 1:
        s = itoa(m * 2) + tr("B/s");
        break;
    case 2:
        d = (m * 2) / 1024;
        sprintf(ss,"%.2f",d);
        s = ss + tr("Kib/s");
        break;
    case 3:
        d = (m * 2) / 1024 / 1024;
        sprintf(ss,"%.2f",d);
        s = ss + tr("Mib/s");
        break;
    case 4:
        d = (m * 2) / 1024 / 1024 / 1024;
        sprintf(ss,"%.2f",d);
        s = ss + tr("Gib/s");
        break;
    default:
        printf("error\n");
        break;
    }
    return s;
=======
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
}

void Watchwindow::endproc()
{
    int m = tableWidget->currentRow();
    int n = p.pinfo[m].pid;
    kill(n , 15);

    tableWidget->removeRow(m);
}

<<<<<<< HEAD

=======
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
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
<<<<<<< HEAD
   // printf("%s  %u \n",p.mem_stat.name, p.mem_stat.total);
   // printf("%s  %u \n",p.mem_stat.name2,p.mem_stat.free);
   // printf("Memory : %u%% \n",(p.mem_stat.total - p.mem_stat.free ) * 100 / p.mem_stat.total);
=======
    printf("%s  %u \n",p.mem_stat.name, p.mem_stat.total);
    printf("%s  %u \n",p.mem_stat.name2,p.mem_stat.free);
    printf("Memory : %u%% \n",(p.mem_stat.total - p.mem_stat.free ) * 100 / p.mem_stat.total);
>>>>>>> 11cb5218703e692e5de4f9df9ca6508a293caff9
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




