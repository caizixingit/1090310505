#include "watchwindow.h"
#include "proc.h"

Watchwindow::Watchwindow(QWidget *parent) :
    QDialog(parent)
{
    p.init();
    p.get_df();
    Getminfo();
    Getpinfo_first();
    QTimer * timer = new QTimer(this);
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
    mLayout = new QGridLayout();

    dfwidget = new QWidget();
    dfLayout = new QVBoxLayout();

    endbutton = new QPushButton(tr("结束进程"));
    connect(endbutton, SIGNAL(clicked()), this, SLOT(endproc()));

    PaintTable();

    PaintMem();

    PaintDf();


    tabwidget->addTab(pwidget,tr("进程"));
    tabwidget->addTab(mwidget,tr("资源"));
    tabwidget->addTab(dfwidget,tr("文件系统"));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(tabwidget,1,1,100,100);
    setLayout(mainLayout);

}

void Watchwindow::PaintDf()
{
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::black);
    label_df = new QLabel(tr("文件系统"));
    label_df->setPalette(pe);

    dfWidget = new QTableWidget(p.df_count,7);
    //dfWidget->setWindowTitle("QTableWidget & Item");
    dfWidget->resize(550, 500); //设置表格
    dfWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    dfWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不可更改
    QStringList header;
    header<<tr("设备")<<tr("目录")<<tr("总数")<<tr("空闲")<<tr("可用")<<tr("已用")<<tr("已用%");
    dfWidget->setHorizontalHeaderLabels(header);

    for(int i = 0; i < p.df_count; i++)
    {
        dfWidget->setItem(i,0,new QTableWidgetItem(p.df[i].device));
        dfWidget->setItem(i,1,new QTableWidgetItem(p.df[i].mounted));
        dfWidget->setItem(i,2,new QTableWidgetItem(p.df[i].size));
        dfWidget->setItem(i,3,new QTableWidgetItem(p.df[i].free));
        dfWidget->setItem(i,4,new QTableWidgetItem(p.df[i].useless));
        dfWidget->setItem(i,5,new QTableWidgetItem(p.df[i].used));
        dfWidget->setItem(i,6,new QTableWidgetItem(itoa(p.df[i].percent) + "%"));
    }
    dfLayout->addWidget(label_df);
    dfLayout->addWidget(dfWidget);
    dfwidget->setLayout(dfLayout);
}


void Watchwindow::PaintMem()
{
    QTimer * timers = new QTimer(this);
    timers->start(500);
    connect(timers, SIGNAL(timeout()), this, SLOT(cputimerout()));

    Getcpuinfo();
    p.get_netdev();
    int m = (p.mem_stat.total - p.mem_stat.free ) * 100 / p.mem_stat.total;
    QString s = itoa(m) + QLatin1String("%");
    QString s1 = "%";

    label_cpu = new QLabel(tr("CPU历史"));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::black);
    label_cpu->setPalette(pe);
    label1 = new QLabel(tr("内存总量："));
    label2 = new QLabel(tr("空闲内存："));
    label3 = new QLabel(tr("内存使用率："));
    label4 = new QLabel(tr("cpu使用率："));
    label12 = new QLabel(itoa(p.mem_stat.total));
    label22 = new QLabel(itoa(p.mem_stat.free));
    label32 = new QLabel(s);
    label42 = new QLabel(s1);

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

    mwidget->setLayout(mLayout);
}

void Watchwindow::PaintTable()
{
    Getburden();

    tableWidget = new QTableWidget(p.count,5);
    tableWidget->setWindowTitle("QTableWidget & Item");
    tableWidget->resize(550, 500); //设置表格
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    //tableWidget->setEditTriggers(QAbstractItemView::CurrentChanged);  //不可更改
    QStringList header;
    header<<tr("进程名")<<tr("进程ID")<<tr("状态")<<tr("内存")<<tr("内存%%");
    tableWidget->setHorizontalHeaderLabels(header);
    tablechange();
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可更改
    vLayout->addLayout(burdenLayout);
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
    dwidget->cpu_use[dwidget->cpu_use_ptr] = m1;
    dwidget->cpu_use_ptr = (dwidget->cpu_use_ptr + 1) % 120;
    dwidget->cpu_use_count++;
    QString s = itoa(m) + "%";
    QString s1 = itoa(m1) + "%";
    label12->setText(itoa(p.mem_stat.total));
    label22->setText(itoa(p.mem_stat.free));
    label32->setText(s);
    label42->setText(s1);

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
        sprintf(ss,"%0.2f",d);
        s = ss + tr("Kib/s");
        break;
    case 3:
        d = (m * 2) / 1024 / 1024;
        sprintf(ss,"%0.2f",d);
        s = ss + tr("Mib/s");
        break;
    case 4:
        d = (m * 2) / 1024 / 1024 / 1024;
        sprintf(ss,"%0.2f",d);
        s = ss + tr("Gib/s");
        break;
    default:
        printf("error\n");
        break;
    }
    return s;
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
   // printf("%s  %u \n",p.mem_stat.name, p.mem_stat.total);
   // printf("%s  %u \n",p.mem_stat.name2,p.mem_stat.free);
   // printf("Memory : %u%% \n",(p.mem_stat.total - p.mem_stat.free ) * 100 / p.mem_stat.total);
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




