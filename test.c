#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

typedef struct PACKED
{
char name[20];      //定义一个char类型的数组名name有20个元素
unsigned int user; //定义一个无符号的int类型的user
unsigned int nice; //定义一个无符号的int类型的nice
unsigned int system;//定义一个无符号的int类型的system
unsigned int idle; //定义一个无符号的int类型的idle
}CPU_OCCUPY;

typedef struct PACKED1       //定义一个mem occupy的结构体
{
char name[20];      //定义一个char类型的数组名name有20个元素
unsigned long total; 
char name2[20];
unsigned long free;                       
}MEM_OCCUPY;

typedef struct PACKED2
{
char name[40];
char state;
char pid[10];
char size[20]; 
char uid[20];
}ProcInfo;

ProcInfo pinfo[1000];
int count = 0;

get_memoccupy (MEM_OCCUPY * mem) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;          
    int n;             
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

int cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n) 
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
    printf("cpu: %u%\n",cpu_use);
    return cpu_use;
}

get_cpuoccupy (CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O
{   
    FILE *fd;         
    int n;            
    char buff[256]; 
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy=cpust;
                                                                                                               
    fd = fopen ("/proc/stat", "r"); 
    fgets (buff, sizeof(buff), fd);
    
    sscanf (buff, "%s %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle);
    
    fclose(fd);     
}

void getprocinfo(int m) //获得进程的名字和id
{
	FILE * fd;
	char tmp1[100];
	char tmp2[100];
	char buff[256];
	char addr[256] = "/proc/";
	while(m > 0)
	{
		addr[strlen(addr)] =  m % 10 + '0';
		m = m / 10;
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
			strncpy(pinfo[count].name,tmp2,20);
		else if(tmp1 == strstr(tmp1,"Pid"))
			strncpy(pinfo[count].pid,tmp2,10);
		else if(tmp1 == strstr(tmp1,"State")) 
			pinfo[count].state = tmp2[0];
		else if(tmp1 == strstr(tmp1,"Uid")) 
			strncpy(pinfo[count].uid,tmp2,20);
	}
	count++;
}

output()
{
	int i =0;
	for(i = 0; i < count; i++)
	{
		if(strcmp(pinfo[i].uid,"0") != 0)
			printf("%s %s %c %s\n",pinfo[i].name, pinfo[i].pid, pinfo[i].state, pinfo[i].uid);
	}
}

int main()
{
    CPU_OCCUPY cpu_stat1;
    CPU_OCCUPY cpu_stat2;
    MEM_OCCUPY mem_stat;
    int cpu;
    //获取内存
    get_memoccupy ((MEM_OCCUPY *)&mem_stat);
    printf("%s  %d \n",mem_stat.name, mem_stat.total);
    printf("%s  %d \n",mem_stat.name2,mem_stat.free);
    printf("Memory : %d% \n",(mem_stat.total - mem_stat.free ) * 100 / mem_stat.total);
    //第一次获取cpu使用情况
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
    sleep(10);
    
    //第二次获取cpu使用情况
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);
    
    //计算cpu使用率
    cpu = cal_cpuoccupy ((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
	int i = 0;
	for(i = 0; i < 4000; i++)
		getprocinfo(i);
    output();
    return 0;
} 
