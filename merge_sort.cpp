// merge_sort.c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/* 声明变量 */
int *array;

int ReadLength(char *fName);
int *ReadFile(char *fName, int length);
void Merge(int arr[], int L, int mid, int R);
void *MergeSort(void *arg);

/* 主函数 */
int main(int argc, char *argv[])
{
    char *fname = argv[1]; /* 从命令行中读取数据文件 */

    /* 获取数据的长度 从指定的文件中计算数据长度：  */
    int length = ReadLength(fname);

    /* 从数据文件中读取数据 */
    array = ReadFile(fname, length);

    int arg[2];
    arg[0] = 0;
    arg[1] = length - 1;

    /* 创建线程执行归并排序 
    创建新的线程调用归并排序算法 merge_sort。*/
    pthread_t tid;
    pthread_create(&tid, NULL, MergeSort, arg);

    /* 进程同步 
在主程序中为了打印已排序好的数据，必须等待排序线程执行完毕，才能打印到已排好的数组，所以需要线程同步。 */
    pthread_join(tid, NULL);

    /* 打印已排序数组 */
    int j;
    for(int i = 0; i < length; i++){
        if(i) printf(" ");
        printf("%d", array[i]);
    }
    printf("\n");
    return 0;
}

/* 求文件的行数（也就是数据量）*/
int ReadLength(char *fName)
{
    FILE* fd = fopen(fName, "rt");
    char line[80];
    int length = 0;
    while (fgets(line, sizeof line, fd) != NULL)
    {
        length++;
    }
    return length;
}

/* 用于从文件读取数据 */
int *ReadFile(char *fName, int length)
{
    FILE* fd = fopen(fName, "rt");
    char buff[80];

    //数组
    int *input = (int *)malloc(length * sizeof(int));
    int idx = 0;

    int x;
    while (fgets(buff, sizeof buff, fd) != NULL)
    {
        sscanf(buff, "%d", &x);
        input[idx++] = x;
    }
    fclose(fd);
    return input;
}

void Merge(int arr[], int L, int mid, int R)
{
}

void *MergeSort(void *arg)
{
    int *arr = array;
    int *argu = (int *)arg;
    int L = argu[0];
    int R = argu[1];
    if (L < R)
    {
        pthread_t tid1;
        pthread_t tid2;

        int mid = L + R >> 1;
        int arg1[2];
        arg1[0] = L;
        arg1[1] = mid;

        int arg2[2];
        arg2[0] = mid + 1;
        arg2[1] = R;

        pthread_create(&tid1, NULL, MergeSort, arg1);
        pthread_create(&tid2, NULL, MergeSort, arg2);

        //等待线程结束
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

        int Tmp[R - L + 1];
        int idx = 0, i = L, j = mid + 1;
        while (i <= mid && j <= R)
        {
            if (arr[i] < arr[j])
                Tmp[idx++] = arr[i++];
            else
                Tmp[idx++] = arr[j++];
        }
        while (i <= mid)
        {
            Tmp[idx++] = arr[i++];
        }
        while (j <= R)
        {
            Tmp[idx++] = arr[j++];
        }
        for (int i = L, j = 0; i <= R; i++, j++)
        {
            arr[i] = Tmp[j];
        }
        
        pthread_exit(0); //退出线程
    }
    return NULL;
}
