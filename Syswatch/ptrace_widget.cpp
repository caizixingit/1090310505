#include "ptrace_widget.h"

Ptrace_widget::Ptrace_widget(QWidget *parent) :
    QWidget(parent)
{
    sys_count = 0;
    get_syscall_name();
    int i = 0;
    for(i = 0; i < 512; i++)
        syscall_id[i] = 0;

    label = new QLabel(tr("输入路径"));
    edit = new QLineEdit();
    button = new QPushButton(tr("跟踪"));
    tabwidget = new QTabWidget();
    tableWidget = new QTableWidget();
    layout = new QGridLayout();
    connect(button, SIGNAL(clicked()), this, SLOT(begin_trace()));

    PaintTable();
    tabwidget->addTab(tableWidget,tr("系统调用"));

    layout->addWidget(label,0,0);
    layout->addWidget(button,0,2);
    layout->addWidget(edit,0,1);
    layout->addWidget(tabwidget,1,0,3,3);

    setLayout(layout);
}

void Ptrace_widget::get_syscall_name()
{
    FILE * fd;
    fd = fopen("/usr/include/i386-linux-gnu/asm/unistd_32.h","r");
    if(fd < 0)
        printf("open unistd.h error \n");
    else
    {
            char buf[256];
            char tmp1[100];
            char tmp2[100];
            int num;
            while(fgets(buf,sizeof(buf),fd) != NULL)
            {
                    if(buf == strstr(buf,"#define"))
                    {
                            sscanf(buf,"%s %s %d",tmp1, tmp2, &num);
                            syscallname[num] = tmp2;
                            num = 511;
                    }
            }
    }
}

void Ptrace_widget::ptrace_path(char * path)
{

    pid_t child;
    long orig_eax;
    int status;
    child = fork();
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl(path,NULL, NULL);
    }
    else
    {
        while(1)
        {
             wait(&status);
             if(WIFEXITED(status))
                  break;
             orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
             syscall_id[orig_eax] += 1 ;
             ptrace(PTRACE_SYSCALL,child, NULL, NULL);
         }
         ptrace(PTRACE_DETACH, child, NULL, NULL);
         int i = 0;
         for(i = 0; i < 511; i++)
         {
              if(syscall_id[i] == 0)
                  continue;
              else
              {
                  //printf("The child made a system call %d: %d \n",i, syscall_id[i]);
                  syscall[sys_count].id = i;
                  syscall[sys_count].name = syscallname[i];
                  syscall[sys_count].times = syscall_id[i];
                  sys_count++;
              }
          }
         printf("%d %s %d \n",syscall[sys_count - 1].id,
                syscall[sys_count - 1].name.unicode(),
                syscall[sys_count - 1].times);
     }
}

void Ptrace_widget::PaintTable()
{

    tableWidget = new QTableWidget(sys_count,3);
    tableWidget->setWindowTitle("QTableWidget & Item");
    tableWidget->resize(350, 300); //设置表格
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可更改
    QStringList header;
    header<<tr("调用id")<<tr("调用名称")<<tr("调用次数");
    tableWidget->setHorizontalHeaderLabels(header);
}

void Ptrace_widget::tablechange()
{
    tableWidget->setRowCount(sys_count);
    for(int i = 0; i < sys_count; i++)
    {
        QString s[2];
        s[0] = itoa(syscall[i].id);
        s[1] = itoa(syscall[i].times);

        tableWidget->setItem(i,0,new QTableWidgetItem(s[0]));
        tableWidget->setItem(i,1,new QTableWidgetItem(syscall[i].name));
        tableWidget->setItem(i,2,new QTableWidgetItem(s[1]));

    }
}
QString Ptrace_widget::itoa(int m)
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

void Ptrace_widget::begin_trace()
{
    QString s = edit->displayText();
    QByteArray ba = s.toLatin1();
    char * ss = ba.data();
    printf("%d %s\n", ss);
    ptrace_path(ss);
    tablechange();
}
