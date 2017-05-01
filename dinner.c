#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
// #include <sys/type.h>

#define PHINUM 5
sem_t semChopsticks[PHINUM];
sem_t room;

void* philosopher(void * i)
{
    int k = 30;
    while(k)
    {
        // pid_t tid = gettid();]
        sleep(0.15);
        int id =*((int *)i);
        printf("i am philosopher %d and I am thinkING\n",id);
        sem_wait(&room);
        
        printf("i am philosopher %d i've in room and try to get chopsitcks\n", id);
        sem_wait(&semChopsticks[id-1]);
        sem_wait(&semChopsticks[(id)%5]);
        sleep(0.15);
        printf("i am philosopher %d I AM  EATING!!!!!!!!!\n",id);
        sem_post(&semChopsticks[(id)%5]);
        sem_post(&semChopsticks[id-1]);
        printf("i am philosopher %d after  EATING i give back\n",id);
        sem_post(&room);
        k--;
    }
}

int main()
{

    for (int i = 0; i < PHINUM; i++)
    {
        sem_init(&semChopsticks[i], 0, 1);
    }
    sem_init(&room, 0, PHINUM-1);

    int i1=1,i2=2,i3=3,i4=4,i5=5;
    pthread_t pt[PHINUM];
    pthread_create(&pt[0],NULL,philosopher, &i1);
    pthread_create(&pt[1],NULL,philosopher, &i2);
    pthread_create(&pt[2],NULL,philosopher, &i3);
    pthread_create(&pt[3],NULL,philosopher, &i4);
    pthread_create(&pt[4],NULL,philosopher, &i5);

    pthread_join(pt[0],NULL);
    pthread_join(pt[1],NULL);
    pthread_join(pt[2],NULL);
    pthread_join(pt[3],NULL);
    pthread_join(pt[4],NULL);

    printf("we eat happily!!");
}

// Program diningphilosophers;
// Var  fork:array [0..4] of semaphore (:= 1);
//       room : semaphore (:= 4);
// i : integer;
// procedure philosopher (i : integer);
// {
// While(1)
// {
// think; 
//   wait(room); /*第5位哲学家将被room信号量阻塞*/
//        wait(fork[i]);
//        wait(fork[(i + 1) mod 5]);
//        eat; 
//        signal(fork[(i + 1) mod 5]);
//        signal(fork[i]);    
//   signal(room);/*唤醒被阻塞的第5位哲学家*/
// }
