#include "proc.h"

Proc::Proc()
{
    cpu = 0;
    cpu_ptr = 0;
}


ProcInfo pinfo[1000];
int count = 0;

void Proc::init()
{
    cpu = 0;
    cpu_ptr = 0;
    count = 0;
}

void Proc::get_memoccupy (MEM_OCCUPY * mem) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;
    char buff[256];
    MEM_OCCUPY *m;
    m=mem;

    fd = fopen ("/proc/meminfo", "r");

    fgets (buff, sizeof(buff), fd);
    sscanf (buff, "%s %u", m->name, &m->total);
    fgets (buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里
    sscanf (buff, "%s %u", m->name2, &m->free);
    fclose(fd);     //关闭文件fd
}

int Proc::cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n)
{
    unsigned long od, nd;
    unsigned long id, sd;
    int cpu_use = 0;

    od = (unsigned long) (o->user + o->nice + o->system +o->idle);//第一次(用户+优先级+系统+空闲)的时间(该时间是从系统启动到现在的时间，所以要相减)
    nd = (unsigned long) (n->user + n->nice + n->system +n->idle);//第二次(用户+优先级+系统+空闲)的时间

    id = (unsigned long) (n->user - o->user);    //用户第一次和第二次的时间之差
    sd = (unsigned long) (n->system - o->system);//系统第一次和第二次的时间之差
    if((nd-od) != 0)
        cpu_use = (int)((sd+id)*100)/(nd-od); //((用户+系统)乖100)除(第一次和第二次的时间差)
    else cpu_use = 0;
    printf("cpu: %d%% \n",cpu_use);
    return cpu_use;
}

void Proc::get_cpuoccupy (CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;
    char buff[256];
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy=cpust;

    fd = fopen ("/proc/stat", "r");
    fgets (buff, sizeof(buff), fd);

    sscanf (buff, "%s %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle);

    fclose(fd);
}

int Proc::findname(char name[],int id)
{
        int i = 0;
        for(i = 0; i < count; i++)
        {
                if(strstr(pinfo[i].name,name) != NULL)
                {
                        pinfo[i].Tgid[pinfo[i].Tcount] = id;
                        pinfo[i].Tcount ++;
                        return 1;
                }
        }
        return 0;
}



void Proc::getprocinfo(int m) //获得进程的名字和id
{

        FILE * fd;
        char tmp1[100];
        char tmp2[100];
        char buff[256];
        char addr[256] = "/proc/";
        int digit = 0;
        int n = m;
        int id = m;
        while(n > 0)
        {
                n = n / 10;
                digit++;
        }
        while(m > 0)
        {
                addr[strlen(addr) + digit - 1] =  m % 10 + '0';
                m = m / 10;
                digit--;
        }
        strcat(addr,"/status");
        fd = fopen(addr,"r");
        if(fd <= 0)
                return;
        while(fgets(buff,sizeof(buff),fd) != NULL)
        {
                sscanf (buff, "%s %s", tmp1,tmp2);
                //printf("%s,%s",tmp1,tmp2);
                if(tmp1 == strstr(tmp1,"Name"))
                {
                        if(!findname(tmp2,id))
                        {
                                strncpy(pinfo[count].name,tmp2,20);
                                continue;
                        }
                        else
                        {
                                count--;
                                break;
                        }
                }
                else if(tmp1 == strstr(tmp1,"Pid"))
                {
                        pinfo[count].pid = atoi(tmp2);
                        continue;
                }
                else if(tmp1 == strstr(tmp1,"State"))
                {
                        pinfo[count].state = tmp2[0];
                        continue;
                }
                else if(tmp1 == strstr(tmp1,"Uid"))
                {
                    if(atoi(tmp2) != 0)
                    {
                        pinfo[count].uid = atoi(tmp2);
                        continue;
                    }
                    else
                    {
                    count--;
                    break;
                    }
                }
                else if(tmp1 == strstr(tmp1,"VmRSS"))
                {
                    pinfo[count].mem = atoi(tmp2) / 4;
                }
        }
        fclose(fd);
        count++;
}

void Proc::proc_track(int pid)
{
        int i = 0, j = 0;
        for(i = 0; i < count; i++)
        {
                if(pinfo[i].pid == pid)
                        break;
        }
        if(i == count)
                printf("Can't find pid : %d \n",pid);
        else
        {
                for(j = 0; j < pinfo[i].Tcount; j++)
                        printf("\nChild proc of %d is %d ",pid,pinfo[i].Tgid[j]);
        }
}
