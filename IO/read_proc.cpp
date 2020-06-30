#include <cstdio>
#include <string.h>
#include <dirent.h>

void getOsInfo()
{
    FILE *fp = fopen("/proc/version", "r");
    if (fp == NULL)
    {
        printf("failed to open version\n");
        //exit();
    }
    char Info[1000];
    while (!feof(fp)) //文件还未读到结束
    {
        memset(Info, 0, sizeof(Info));
        //系统在末尾加上\0, 所以需要-1
        fgets(Info, sizeof(Info) - 1, fp);
        printf("%s\n", Info);
    }
    fclose(fp);
}

void getCupInfo()
{
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL)
    {
        printf("failed to open cpuinfo\n");
        //exit();
    }
    char Info[1000];
    while (!feof(fp)) //文件还未读到结束
    {
        memset(Info, 0, sizeof(Info));
        //系统在末尾加上\0, 所以需要-1
        fgets(Info, sizeof(Info) - 1, fp);
        printf("%s\n", Info);
    }
    fclose(fp);
}

void getMemoryInfo()
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        printf("failed to open meminfo\n");
        //exit();
    }
    char Info[1000];
    while (!feof(fp)) //文件还未读到结束
    {
        memset(Info, 0, sizeof(Info));
        //系统在末尾加上\0, 所以需要-1
        fgets(Info, sizeof(Info) - 1, fp);
        printf("%s\n", Info);
    }
    fclose(fp);
}

void GetAllPid()
{
    char filepath[50];
    FILE *fp;
    char buf[100000];
    char cur_task_name[50];

    DIR *dir = opendir("/proc");
    struct dirent *ptr;

    if (dir == NULL)
    {
        printf("failed to open proc\n");
        //exit();
    }
    while ((ptr = readdir(dir)) != NULL)
    {
        if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
            continue;
        if (DT_DIR != ptr->d_type)
            continue;
        sprintf(filepath, "/proc/%s/status", ptr->d_name);
        fp = fopen(filepath, "r");
        if (fp != NULL)
        {
            //读取第一行 Name
            if( fgets(buf, sizeof(buf) - 1, fp)== NULL ){
                fclose(fp);
                continue;
            }
            sscanf(buf, "%*s %s", cur_task_name);
            //%*s会被忽略，不会被获取
            printf("%s\n", cur_task_name);
            fclose(fp);
        }
    }
    closedir(dir);
}

int main()
{
    // getOsInfo();
    // getCupInfo();
    // getMemoryInfo();
    GetAllPid();
}