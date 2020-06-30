#include <iostream>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>

using namespace std;

union semun{  
    int val;  
    struct semid_ds *buf;  
    unsigned short *array;  
    struct seminfo  *__buf;
};  

//获取与key关联的信号量
int AcquireBinarySemaphore(key_t key, int sem_flags){
    return semget(key, 1, sem_flags);
}

//初始化信号量
int InitializeBinarySemaphore(int semid)
{
    //单一元素，信号量初始值为0
    unsigned short int values[1] = {0};
    union semun needed = {.array = values};
    return semctl(semid, 0, SETALL, needed);
}

int ReleaseBinarySemaphore(int semid){
    union semun ignored;
    return semctl(semid, 1, IPC_RMID, ignored);
}
//等待二元信号量
int WaitBinarySemaphore(int semid){
    struct sembuf ops[1];
    ops[0].sem_num = 0;
    ops[0].sem_op = -1;
    ops[0].sem_flg = SEM_UNDO;
    return semop(semid, ops, 1);
}
//发布二元信号量
int PostBinarySemaphore(int semid){
    struct sembuf ops[1];
    ops[0].sem_num = 0;
    ops[0].sem_op = 1;
    ops[0].sem_flg = SEM_UNDO;
    return semop(semid, ops, 1);
}

int main()
{   
    //键值key管理信号量集
    key_t sem_key = ftok("Linux_Cpp/fork_sem", 1);//生成key

    //creat创建信号量集  excl唯一地信号量集  0755信号量权限
    //int sem_id = semget(sem_key, 1, IPC_CREAT|IPC_EXCL|0755);
    int sem_id = AcquireBinarySemaphore(sem_key, IPC_CREAT|IPC_EXCL|0755);
    if(sem_id == -1) exit(1);
    int state = InitializeBinarySemaphore(sem_id);
    if(state == -1) exit(1);

    pid_t child_pid = fork();
    if (child_pid == 0)
    {
        //子进程
        cout << "child PID is " << (int)getpid() << "\n";
        sleep(3);
        PostBinarySemaphore(sem_id);
    }
    else
    {
        WaitBinarySemaphore(sem_id);
        cout << "parent PID is " << (int)getpid() << "\n";
        ReleaseBinarySemaphore(sem_id);
    }
    return 0;
}