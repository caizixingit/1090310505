#ifndef PROC_H
#define PROC_H

#include <QDialog>
#include <QMessageBox>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <mntent.h>
#include <string.h>
#include <sys/vfs.h>

class CPU_OCCUPY
{
public:
        char name[20];      //定义一个char类型的数组名name有20个元素
        unsigned int user; //定义一个无符号的int类型的user
        unsigned int nice; //定义一个无符号的int类型的nice
        unsigned int system;//定义一个无符号的int类型的system
        unsigned int idle; //定义一个无符号的int类型的idle
};

class MEM_OCCUPY      //定义一个mem occupy的结构体
{
public:
        char name[20];      //定义一个char类型的数组名name有20个元素
        unsigned int total;
        char name2[20];
        unsigned int free;
};

class ProcInfo
{
public:
        char name[40];
        char state;
        int pid;
        int uid;
        int Tgid[64];
        int Tcount;
        int mem;
};

class NetDev
{
public:
    int rcv_lo;
    int rcv_eth0;
    int rcv_wlan0;
    int trmt_lo;
    int trmt_eth0;
    int trmt_wlan0;
};


class DF
{
public:
    QString device;
    QString size;
    QString free;
    QString useless;
    QString used;
    QString mounted;
    double percent;
};

class Basic
{
public:
    QString cpuinfo[4];
    int cpu_count;
    QString hz[4];
    QString version;
    QString system;
};

class Proc
{
public:
    Proc();
    CPU_OCCUPY cpu_stat[2];
    int cpu_ptr;

    NetDev netdev[2];
    int nd_ptr;

    MEM_OCCUPY mem_stat;
    int count;
    ProcInfo pinfo[10000];
    int cpu;

    DF df[20];
    int df_count;

    Basic basic;

    char burden1[10];
    char burden5[10];
    char burden15[10];

    int netdev_rcv;
    int netdev_trmt;

    void get_memoccupy (MEM_OCCUPY * mem);
    int cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n);
    void get_cpuoccupy (CPU_OCCUPY *cpust);
    int findname(char name[],int id);
    void getprocinfo(int m);
    void proc_track(int pid);
    void init();
    void get_burden();
    void get_netdev();
    void cal_netdev(int * m, int * n);

    void get_df();
    void get_basic();
    QString getsize(unsigned long m, unsigned long n);

    void test(QString s);

};

#endif // PROC_H
