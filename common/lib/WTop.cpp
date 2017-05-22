#if defined(__GNUC__) || defined(linux) ||defined(__linux)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <string>
#include<unistd.h>
#include<fcntl.h>
#include<ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h> 
#include <vector>
#include <assert.h>
#include<iostream>
#include "Public.h"
using namespace std;

#define CK_TIME 1

struct FileAttribute
{
  string path;
  string name;
  unsigned long long size;
  time_t  modify_timestamp;
  bool    is_dir;
};

int EnumFile(vector<FileAttribute> &file_array, string _dir)
{
  DIR* dir=opendir(_dir.c_str());  //(".")
  if(dir == NULL)
    return -1;

  struct dirent* entry;
  while((entry=readdir(dir)))
  {
    if( strcmp( entry->d_name,".") ==0 || strcmp( entry->d_name,"..") ==0 )
      continue;
    FileAttribute fi;
    fi.name = entry->d_name;
    fi.is_dir = false;
    string path;

    if(_dir=="/"||(_dir.rfind("/")+1)>=_dir.length())
      path=_dir+fi.name;
    else
      path = _dir+"/"+fi.name;
    struct stat statbuf;
    if (stat( path.c_str(),&statbuf ) < 0)
    {
      closedir(dir);
      printf("stat error ! message: %s\n",strerror(errno));
      return 0;
    }

    if (S_ISDIR(statbuf.st_mode))
    {
      fi.is_dir = true;
    }
    fi.size = statbuf.st_size;
    fi.modify_timestamp =statbuf.st_mtime;
    fi.path = path;
    file_array.push_back(fi);                    
  }
  closedir(dir);
  return file_array.size();
}

char * skip_token(const char *p)
{
  while (isspace(*p)) p++;
  while (*p && !isspace(*p)) p++;
  return (char *)p;
}

int get_sys_mem(char *mem)
{
  int tm;//fm,bm,cm,ts,fs;
  char buffer[4096+1];
  int fd, len;
  char *p;
  //fd = open("/proc/meminfo", O_RDONLY);
  if((fd = open("/proc/meminfo", O_RDONLY)) < 0)
  {
    return -1;
  }
  len = read(fd, buffer, sizeof(buffer)-1);
  close(fd);

  buffer[len] = '\0';
  p = buffer;
  p = skip_token(p); 
  tm = strtoul(p, &p, 10); /* total memory */
  return tm;
}

int get_phy_mem(pid_t pid,char* ph)
{
  char file[64] = {0};//文件名
  FILE *fd;         //定义文件指针fd
  char line_buff[256] = {0};  //读取行的缓冲区
  sprintf(file,"/proc/%d/status",pid);
  if((fd = fopen (file, "r"))==NULL)
  {
    return -1;
  }
  //获取vmrss:实际物理内存占用
  for (int i =0;i<15;i++)
  {
    fgets (line_buff, sizeof(line_buff), fd);
  }
  fgets (line_buff, sizeof(line_buff), fd);//读取VmRSS这一行的数据,VmRSS在第15行
  fclose(fd);     //关闭文件fd
  int vmrss;//存放内存峰值大小
  sscanf (line_buff, "%*[^ ] %d",&vmrss);
  return vmrss;
}

int get_shr_mem(pid_t pid)
{
  char file[64] = {0};//文件名
  FILE *fd;         //定义文件指针fd
  char line_buff[256] = {0};  //读取行的缓冲区
  sprintf(file,"/proc/%d/statm",pid);
  if((fd = fopen (file, "r"))==NULL)
  {
    return -1;
  }
  //获取vmrss:实际物理内存占用
  int vmshr = 0;//存放内存峰值大小
  fgets (line_buff, sizeof(line_buff), fd);
  fclose(fd);
  sscanf (line_buff, "%*[^ ] %*[^ ] %d",&vmshr);
  return vmshr;
}

int get_process_time(pid_t pid,int tid)
{    
  char szStatStr[1024];
  char pname[64];
  char state;
  int ppid,pgrp,session,tty,tpgid;
  unsigned int    flags,minflt,cminflt,majflt,cmajflt;
  int utime,stime,cutime,cstime,counter,priority;
  unsigned int  timeout,itrealvalue;
  int           starttime;
  unsigned int  vsize,rss,rlim,startcode,endcode,startstack,kstkesp,kstkeip;
  int signal,blocked,sigignore,sigcatch;
  unsigned int  wchan;

  char file_stat [1024];
  if(tid==0)
  {
    sprintf( file_stat,"/proc/%d/stat",pid );
  }else if(tid!=-1) 
  {
    sprintf( file_stat,"/proc/%d/task/%d/stat",pid,tid );
  }

  //printf("open file %s\n",file_stat);

  FILE* fid;
  //fid = fopen(file_stat,"r");
  if((fid = fopen (file_stat, "r"))==NULL)
  {
    return -1;
  }

  fgets(szStatStr,sizeof(szStatStr),fid);

  fclose(fid);

  //printf("+++szStatStr=%s\n",szStatStr);

  sscanf (szStatStr, "%u", &pid);
  char  *sp, *t;
  sp = strchr (szStatStr, '(') + 1;
  t = strchr (szStatStr, ')');
  strncpy (pname, sp, t - sp);

  sscanf (t + 2, "%c %d %d %d %d %d %u %u %u %u %u %d %d %d %d %d %d %u %u %d %u %u %u %u %u %u %u %u %d %d %d %d %u",
    /*     1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33*/
    &state,&ppid,&pgrp,&session,&tty,&tpgid,&flags,&minflt,&cminflt,&majflt,&cmajflt,&utime,&stime,&cutime,&cstime,&counter,
    &priority,&timeout,&itrealvalue,&starttime,&vsize,&rss,&rlim,&startcode,&endcode,&startstack,
    &kstkesp,&kstkeip,&signal,&blocked,&sigignore,&sigcatch,&wchan);
  /*printf("-------%c %d %d %d %d %d %u %u %u %u %u -%d -%d -%d -%d %d %d %u %u %d %u %u %u %u %u %u %u %u %d %d %d %d %u",
  state,ppid,pgrp,session,tty,tpgid,flags,minflt,cminflt,majflt,cmajflt,utime,stime,cutime,cstime,counter,
  priority,timeout,itrealvalue,starttime,vsize,rss,rlim,startcode,endcode,startstack,
  kstkesp,kstkeip,signal,blocked,sigignore,sigcatch,wchan);
  */
  //printf("+++%lu %lu %lu %lu\n",utime,stime,cutime,cstime);

  int p_cpu=utime+stime+cutime+cstime;
  return p_cpu;
}

namespace Public
{
  bool GetCpuMem(int pid, float &cpu ,float &mem, int& nThreadNum, int tid)
  {
    FILE *fp;
    char buf[128];
    char tcpu[7];
    char cmem[256];

    unsigned int  user,nice,sys,idle,iowait,irq,softirq,steal;

    unsigned int  all1,all2;

    unsigned int   us1,ni1,sy1,id1,io1,ir1,so1,st1;
    unsigned int   us2,ni2,sy2,id2,io2,ir2,so2,st2;

    int  p_cpu1 = 0,p_cpu2 = 0;

    float usage,niage,syage,idage,ioage,irage,soage,stage;

    if((fp = fopen ("/proc/stat", "r"))==NULL)
    {
      return false;
    }
    fgets(buf,sizeof(buf),fp);
    fclose(fp);

    sscanf(buf,"%s%d%d%d%d%d%d%d%d",tcpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq,&steal);

    //printf("%s,%d,%d,%d,%d,%d,%d,%d,%d\n",tcpu,user,nice,sys,idle,iowait,irq,softirq,steal);

    all1 = user+nice+sys+idle+iowait+irq+softirq+steal;

    us1=user;ni1=nice;sy1=sys;id1=idle;
    io1=iowait;ir1=irq;so1=softirq;st1=steal;
    //=============================================

    char file_dir[256];
    sprintf(file_dir,"/proc/%d/task",(int)pid);
    string _dir=file_dir;
    //printf("-----%s\n",file_dir);
    vector<FileAttribute> file_array;
    int file_sum=EnumFile(file_array,_dir);
    if (file_sum < 0)
    {
      return false;
    }
    nThreadNum = file_sum;
    unsigned int a[file_sum];
    unsigned int b[file_sum];
    unsigned int b2[file_sum];
    int i=0;

    if(tid==-1)
    {
      for(vector<FileAttribute>::iterator it=file_array.begin();it!=file_array.end();it++)
      {
        _dir=(*it).name;
        //cout<<"_dir="<<_dir<<endl;
        a[i]=atoi( _dir.c_str());
        i++;
      }
      for(int j=0;j<file_sum;j++)
      {
        b[j]=get_process_time( pid,a[j]);

      }
    }else
    {
      p_cpu1= get_process_time( pid, tid);
      if (p_cpu1 < 0)
      {
        return false;
      }
    }
    //===========================================
    /*第二次取数据*/
    sleep(CK_TIME);
    memset(buf,0,sizeof(buf));
    tcpu[0] = '\0';
    user=nice=sys=idle=iowait=irq=softirq=steal=0;
    if((fp = fopen ("/proc/stat", "r"))==NULL)
    {
      return false;
    }
    fgets(buf,sizeof(buf),fp);
    fclose(fp);

    sscanf(buf,"%s%d%d%d%d%d%d%d%d",tcpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq,&steal);

    //printf("%s,%d,%d,%d,%d,%d,%d,%d,%d\n",tcpu,user,nice,sys,idle,iowait,irq,softirq,steal);

    us2=user;ni2=nice;sy2=sys;id2=idle;
    io2=iowait;ir2=irq;so2=softirq;st2=steal;
    all2 = user+nice+sys+idle+iowait+irq+softirq+steal;

    usage =(float)((us2-us1)+(ni2-ni1))/(all2-all1)*100 ;
    syage=(float)((sy2-sy1)+(ir2-ir1)+(so2-so1))/(all2-all1)*100 ;

    idage=(float)(id2-id1)/(all2-all1)*100;
    niage=(float)(ni2-ni1)/(all2-all1)*100;
    ioage=(float)(io2-io1)/(all2-all1)*100;
    irage=(float)(ir2-ir1)/(all2-all1)*100;
    soage=(float)(so2-so1)/(all2-all1)*100;
    stage=(float)(so2-so1)/(all2-all1)*100;

    if(tid==-1)
    {
      for(int j=0;j<file_sum;j++)
      {
        b2[j]=get_process_time( pid,a[j]);
      }
    }else
    {
      p_cpu2= get_process_time( pid, tid);
      if (p_cpu2 < 0)
      {
        return false;
      }
    }

    int NUM_PROCS = sysconf(_SC_NPROCESSORS_CONF);
    //printf("======%d",NUM_PROCS);

    float prcpu[file_sum];
    if(tid==-1)
    {
      for(int j=0;j<file_sum;j++)
      {
        prcpu[j]=(float)(b2[j]-b[j])/(all2-all1)*NUM_PROCS*100;
      }
    }
    else
    {
      cpu = (float)(p_cpu2 - p_cpu1) * 1.0/(all2-all1)*NUM_PROCS*100;
    }

    char ph[256];
    //long page_size = sysconf(_SC_PAGESIZE)>>10;
    //mem = (get_phy_mem(pid,ph)*page_size) * 1.0 /get_sys_mem(cmem)*100;
    int nPhyMem = get_phy_mem(pid,ph);
    if (nPhyMem < 0)
    {
      return false;
    }
    //int nShrMem = get_shr_mem(pid);
    int nSysMem = get_sys_mem(cmem);
    if (nSysMem < 0)
    {
      return false;
    }
    //mem = ((nPhyMem - nShrMem) * page_size) * 1.0 /nSysMem * 100;
    mem = nPhyMem * 1.0 / nSysMem * 100;
    return true;
  }

  bool GetPidByName(int *pid, const char *task_name)
  {
    #define BUF_SIZE 1024
    DIR *dir;  
    struct dirent *ptr;  
    FILE *fp;  
    char filepath[50];  
    char cur_task_name[50];  
    char buf[BUF_SIZE];  
    bool bRes =false;
    dir = opendir("/proc");   
    if (NULL != dir)  
    {  
      while ((ptr = readdir(dir)) != NULL) //循环读取/proc下的每一个文件/文件夹  
      {  
        //如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过  
        if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))  
          continue;  
        if (DT_DIR != ptr->d_type)  
          continue;  

        sprintf(filepath, "/proc/%s/status", ptr->d_name);//生成要读取的文件的路径  
        fp = fopen(filepath, "r");  
        if (NULL != fp)  
        {  
          if( fgets(buf, BUF_SIZE-1, fp)== NULL ){  
            fclose(fp);  
            continue;  
          }  
          sscanf(buf, "%*s %s", cur_task_name);  

          //如果文件内容满足要求则打印路径的名字（即进程的PID）  
          if (!strcmp(task_name, cur_task_name)){  
            sscanf(ptr->d_name, "%d", pid);
            bRes = true;
          }  
          fclose(fp);  
        }  
      }  
      closedir(dir);
    }
    return bRes;
  }
}

#endif