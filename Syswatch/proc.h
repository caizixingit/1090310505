#ifndef PROC_H
#define PROC_H

#include <QDialog>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

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


class Proc
{
public:
    Proc();
    CPU_OCCUPY cpu_stat[2];
    int cpu_ptr;
    MEM_OCCUPY mem_stat;
    int count;
    ProcInfo pinfo[10000];
    int cpu;

    void get_memoccupy (MEM_OCCUPY * mem);
    int cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n);
    void get_cpuoccupy (CPU_OCCUPY *cpust);
    int findname(char name[],int id);
    void getprocinfo(int m);
    void proc_track(int pid);
    void init();



};

#endif // PROC_H
