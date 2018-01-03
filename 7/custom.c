#include <stdio.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
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

int main(){  
    int running = 1;
    void *shm = NULL; 
    struct shared_use_st *shared;
    int semid;
    // key_t key;  
    // key=ftok(".",i);
    semid=semget(KEY,1,IPC_CREAT | 0666);

    int shmid; 
    shmid = shmget((key_t)1999, sizeof(struct  shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1){  
       exit(EXIT_FAILURE);
    }  
    shm = shmat(shmid, 0, 0);  
    if(shm == (void*)-1){  
       exit(EXIT_FAILURE);
    }  
    shared = (struct shared_use_st*)shm;  
        printf("shmid: %d\n", shmid);


   // FILE *stream;
    int fd;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

   // stream = fopen("custom.txt", "w");
   // if (stream == NULL){
   //      printf("wtf\n");
   //     exit(EXIT_FAILURE);
   // }

    fd = open("custom.txt",  O_CREAT|O_RDWR, 0666);
    int i1 = 10;

    int pid;
    if ((pid=fork()) != 0) {
        while(running){  
            if (semaphore_p(semid)) {
                // printf("index:%d,num:%d ", shared->index, shared->num);
                if (shared->num == 0) {
                    semaphore_v(semid); 
                    // usleep(50);
                    printf("index:%d,num:%d ", shared->index, shared->num);
                    sleep(1);
                    continue;
                } else {
                    if (strncmp(shared->Buffer[shared->index], "IT'S THE END", 12) == 0) {
                printf("index:%d,num:%d ", shared->index, shared->num);
                     
                        printf("son end\n");
                        running = 0;
                    } else {
                        printf("sons custom%s", shared->Buffer[shared->index]);
                        shared->num--;
                        // fputs(shared->Buffer[shared->index], stream);
                        write(fd, shared->Buffer[shared->index], strlen(shared->Buffer[shared->index]));
                        fsync(fd);
                        memset(shared->Buffer[shared->index],0,200);
                        shared->index = (shared->index+1)%5;
    
                    }
                }
                semaphore_v(semid); 
            } else {
                semaphore_v(semid); 
                printf("sleep for 1 seconds\n ");  
                usleep(50);
                // sleep(1);
            }
        }   

    } else {
        while(running){  
            if (semaphore_p(semid)) {
                // printf("index:%d,num:%d ", shared->index, shared->num);
                if (shared->num == 0) {
                    semaphore_v(semid); 
                    usleep(50);
                    // sleep(1);
                    continue;
                } else {
                    if (strncmp(shared->Buffer[shared->index], "IT'S THE END", 12) == 0) {
                        
                printf("index:%d,num:%d ", shared->index, shared->num);
                        printf("father end\n");
                        running = 0;
                    } else {
                        printf("father custom%s", shared->Buffer[shared->index]);
                        shared->num--;
                        // fputs(shared->Buffer[shared->index], stream);
                        write(fd, shared->Buffer[shared->index], strlen(shared->Buffer[shared->index]));
                        fsync(fd);
                        
                        memset(shared->Buffer[shared->index],0,200);
                        shared->index = (shared->index+1)%5;                        
                    } 


                }
                semaphore_v(semid); 
            } else {
                semaphore_v(semid); 
                printf("sleep for 1 seconds\n ");  
                usleep(50);
                // sleep(1);
            }
        }
        // pid_return = waitpid(pid_child, NULL, WNOHANG);            
        waitpid(-1, NULL, 0);

    }

    if(shmdt(shm) == -1){
        exit(EXIT_FAILURE);
    }  
    if(shmctl(shmid, IPC_RMID, 0) == -1){
        exit(EXIT_FAILURE);
    }  
    exit(EXIT_SUCCESS);
}

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
    printf("105\n" );

    if (semop(sem_id, &sem_b, 1) == -1) 
   {
        fprintf(stderr, "semaphore_p failed\n");
        return 0;
    }
    // printf("110\n" );
    return 1;
}