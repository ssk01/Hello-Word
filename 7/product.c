#include<stdio.h>
#include<sys/sem.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

// #define IPC_R 000400   
// #define IPC_W 000200   
// #define IPC_M 010000   

#define TEXT_SZ 100
#define KEY (1023)
// #define BUFSIZ 1024
int semaphore_p(int sem_id);
int semaphore_v(int sem_id);
struct shared_use_st  
{  
    int index;
    int num;//为0表示对应的缓冲区未被生产者使用，可分配但不可消费；为1表示对应的缓冲区以被生产者使用，不可分配但可消费
    char Buffer[5][200];//5个字符串缓冲区
}; 

// struct shared_use_st  
// {  
//     int written;//非0：表示可读，0表示可写  
//     char text[TEXT_SZ];//记录写入和读取的文本}; 
// };

int main()  
{  
    int running = 1;  
    void *shm = NULL;  
    struct shared_use_st *shared = NULL;  
    char buffer[BUFSIZ + 1];

    int i = 1024;
    int semid;
    // key_t key;  
    // key=ftok(".",i);
    union semun {
        int val;
        struct semid_ds *buf;
        ushort * array;
    } argument;

    argument.val = 1;

    semid=semget(KEY,1,IPC_CREAT | 0666);
    printf("key is %d, semid is %d\n", KEY, semid);  

    if( semctl(semid, 0, SETVAL, argument) < 0)
    {
       fprintf(stderr, "Cannot set semaphore value.\n");
    }
    else
    {
       fprintf(stderr, "Semaphore %d initialized.\n", KEY);
    }
   
    int shmid;
    shmid = shmget((key_t)1999, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1){    
        printf("shmid: %d\n", shmid);
        exit(EXIT_FAILURE);
    }  

        printf("shmid: %d\n", shmid);

    shm = shmat(shmid, (void*)0, 0);  
    if(shm == (void*)-1){  
        exit(EXIT_FAILURE);
    }  
    printf("Memory attached at %X\n", (int)shm);  
    shared = (struct shared_use_st*)shm;
    // memset(shared, 0, sizeof(struct shared_use_st));

    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    stream = fopen("productN.txt", "r");
    if (stream == NULL)
    {
        printf("wtf\n");
       // exit(EXIT_FAILURE);
    }



   // free(line);
   // fclose(stream);
   // exit(EXIT_SUCCESS);
    int pid;
    if ((pid=fork()) != 0) {
        while(running){  
            if (semaphore_p(semid)) {
                printf("son index:%d,num:%d ", shared->index, shared->num);
                if (shared->num == 5) {
                    semaphore_v(semid); 
                    printf("sleep for 1 seconds\n ");  
                    usleep(800);
                    // sleep(1);
                    continue;
                } else {
                    read = getline(&line, &len, stream);
                    printf("read is: %d",read);
                    if (read == -1) {
                       printf("son read file all");
                       int p = (shared->num + shared->index)%5;
                       // strncpy(shared->Buffer[p], line, read);
                       strcat(shared->Buffer[p], "IT'S THE END\0");
                       // free(line);
                       // line = NULL;
                       shared->num++;
                       // exit(EXIT_SUCCESS);
                       running = 0;
                    }  else {
                       printf("son Retrieved line of length %zu :\n", read);
                       printf("%s", line);
                       int p = (shared->num + shared->index)%5;
                       strncpy(shared->Buffer[p], line, read);
                       free(line);
                       line = NULL;
                       shared->num++;
                    }
                }
                semaphore_v(semid); 
            } else {
                semaphore_v(semid); 
                printf("son sleep for 2 seconds\n ");  
                    usleep(500);
                // sleep(1);
            }
        }
    } else {
        // while(running){  
        //     if (semaphore_p(semid)) {
        //         printf("index:%d,num:%d ", shared->index, shared->num);
        //         if (shared->num == 5) {
        //             semaphore_v(semid); 
        //             printf("father sleep for 1 seconds\n ");  
        //             usleep(800);
        //             // sleep(1);
        //             continue;
        //         } else {
        //             read = getline(&line, &len, stream);
        //             printf("read\n");
        //             if (read == -1) {
        //                printf("father read file all");
        //                int p = (shared->num + shared->index)%5;
        //                // strncpy(shared->Buffer[p], line, read);
        //                strcat(shared->Buffer[p], "IT'S THE END\0");
        //                shared->num++;
        //                // exit(EXIT_SUCCESS);
        //                running = 0;
        //             }  else {
        //                printf("father Retrieved line of length %zu :\n", read);
        //                printf("%s", line);
        //                int p = (shared->num + shared->index)%5;
        //                strncpy(shared->Buffer[p], line, read);
        //                free(line);
        //                line = NULL;
        //                shared->num++;
        //             }
        //         }
        //         semaphore_v(semid); 
        //     } else {
        //         semaphore_v(semid); 
        //         printf("father sleep for 2 seconds\n ");  
        //         usleep(50);
        //         // sleep(2);
        //     }
        // }
        waitpid(-1, NULL, 0);        
    }
    // if(shmdt(shm) == -1) {
    //     exit(EXIT_FAILURE);
    // }  
    // if(shmctl(shmid, IPC_RMID, 0) == -1){
    //     exit(EXIT_FAILURE);
    // }  
    exit(EXIT_SUCCESS);
}



    // int semid;
    // key_t key;  
    // key=ftok(".",i);
    // semid=semget(MYKEY,1,IPC_CREAT|IPC_R | IPC_W| IPC_M);

int semaphore_v(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V操作 */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) 
    {
        fprintf(stderr, "semaphore_v failed\n");
        return 0;
    }
    return 1;
}
int semaphore_p(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P操作 */
    sem_b.sem_flg = SEM_UNDO;
    // printf("105\n" );

    if (semop(sem_id, &sem_b, 1) == -1) 
   {
        fprintf(stderr, "semaphore_p failed\n");
        return 0;
    }
    // printf("110\n" );
    return 1;
}


