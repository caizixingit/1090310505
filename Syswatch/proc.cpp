#include "proc.h"

Proc::Proc()
{
}


void Proc::init()
{
    cpu = 0;
    cpu_ptr = 0;
    count = 0;
    nd_ptr = 0;
    df_count = 0;
}

void Proc::get_df()
{
    FILE * fd;
    struct mntent * mntents;
    struct statfs sfs;
    fd = setmntent("/etc/mtab", "r");
    if(!fd)
    {
        printf("set mount entry error!\n");
        return;
    }
    while(1)
    {
        char * fname;//文件系统名
        char * dir;//文件系统挂载点，文件系统目录
        if(fd)
        {
            mntents = getmntent(fd);
            if(!mntents)
            {
                endmntent(fd);
                break;
            }
        }
        else continue;
        fname = mntents->mnt_fsname;
        dir = mntents->mnt_dir;
        if(strstr(fname,"none"))
            continue;
        df[df_count].device = fname;
        df[df_count].mounted = dir;
        if(statfs(dir, &sfs) != 0)//根据文件系统的挂载点，将该文件系统的信息读取到staffs中
        {
            printf("statfs error\n");
            continue;
        }
        if(sfs.f_blocks > 0 )
        {
            df[df_count].percent = (sfs.f_blocks - sfs.f_bfree) * 100.0 /(double)sfs.f_blocks;
            df[df_count].size = getsize(sfs.f_blocks, sfs.f_bsize);
            df[df_count].useless = getsize(sfs.f_bavail, sfs.f_bsize);
            df[df_count].free = getsize(sfs.f_bfree,sfs.f_bsize);
            df[df_count].used = getsize(sfs.f_blocks - sfs.f_bfree, sfs.f_bsize);
            df_count++;
        }
    }

}

static const unsigned long long G = 1024*1024*1024ull;
static const unsigned long long M = 1024*1024;
static const unsigned long long K = 1024;

QString Proc::getsize(unsigned long m,unsigned long n)
{
    QString s;
    unsigned long long size = m * (unsigned long long) n;
    char ss[20] = "0 B";
    int i = 0;
    double d = 0;
    while(size > 0)
    {
        size = size / K;
        i++;
    }
    size = m * (unsigned long long)n;
    switch(i)
    {
    case 0:
        break;
    case 1:
        sprintf(ss, "%u B", (int)size);
        break;
    case 2:
        d = size / (K * 1.0);
        sprintf(ss,"%0.2f KiB",d);
        break;
    case 3:
        d = size / (M * 1.0);
        sprintf(ss,"%0.2f MiB",d);
        break;
    case 4:
        d = size / (G * 1.0);
        sprintf(ss,"%0.2f GiB",d);
        break;
    default:
        printf("error\n");
        break;
    }
    s = ss;
    return s;
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
 //   printf("cpu: %d%% \n",cpu_use);
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

void Proc::get_netdev()
{
    FILE * fd;
    char buf[256];
    int m[7];
    char t[20];

    fd = fopen("/proc/net/dev","r");
    if(fd < 0)
       printf("Open netdev error\n");
    while(fgets(buf,sizeof(buf),fd) != NULL)
    {
        if(strstr(buf,"lo"))
        {
            sscanf(buf,"%s %d %d %d %d %d %d %d %d %d",t,
                      &netdev[nd_ptr].rcv_lo,m,m + 1,m + 2,m + 3,m + 4,
                       m + 5,m + 6,&netdev[nd_ptr].trmt_lo);
        }//获得网络lo的接受字节和发送字节
        else if(strstr(buf,"eth0"))
        {
            sscanf(buf,"%s %d %d %d %d %d %d %d %d %d",t,
                   &netdev[nd_ptr].rcv_eth0,m,m + 1,m + 2,m + 3,m + 4,
                   m + 5,m + 6,&netdev[nd_ptr].trmt_eth0);

        }//获取eth0网络的接受字节和发送字节
        else if(strstr(buf,"wlan0"))
        {
            sscanf(buf,"%s %d %d %d %d %d %d %d %d %d",t,
                   &netdev[nd_ptr].rcv_wlan0,m,m + 1,m + 2,m + 3,m + 4,
                   m + 5,m + 6,&netdev[nd_ptr].trmt_wlan0);

        }//获取wlan0网络的接受字节和发送字节
    }
    netdev_rcv = netdev[nd_ptr].rcv_lo + netdev[nd_ptr].rcv_eth0 +
                 netdev[nd_ptr].rcv_wlan0;
    netdev_trmt = netdev[nd_ptr].trmt_lo + netdev[nd_ptr].trmt_eth0 +
                  netdev[nd_ptr].trmt_wlan0;
    nd_ptr = (nd_ptr + 1) % 2 ;
}

void Proc::cal_netdev(int * m, int * n)
{
    int rcv0 = 0, trmt0 = 0, rcv1 = 0, trmt1 = 0;
    int ptr = ( nd_ptr + 1 ) % 2;
    rcv0 = netdev[nd_ptr].rcv_lo + netdev[nd_ptr].rcv_eth0 +
           netdev[nd_ptr].rcv_wlan0;
    trmt0 =  netdev[nd_ptr].trmt_lo + netdev[nd_ptr].trmt_eth0 +
             netdev[nd_ptr].trmt_wlan0;
    rcv1 = netdev[ptr].rcv_lo + netdev[ptr].rcv_eth0 +
           netdev[ptr].rcv_wlan0;
    trmt1 =  netdev[ptr].trmt_lo + netdev[ptr].trmt_eth0 +
             netdev[ptr].trmt_wlan0;
    *m = rcv1 -rcv0;
    *n = trmt1 - trmt0;
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

void Proc::get_burden()
{
    FILE * fd;
    char buf[256];
    fd = fopen("/proc/loadavg","r");
    if(fd < 0)
        printf("Error:Open loadavg\n");
    fgets(buf,sizeof(buf),fd);
    sscanf(buf,"%s %s %s", burden1, burden5, burden15);
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
