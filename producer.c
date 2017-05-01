#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFNUM 20
#define producerloops 4
#define consumerloops 3

FILE* fp;

sem_t empty;
sem_t full;
sem_t mutex;

char *buffer[BUFNUM];
int fill = 0;
int use = 0;

int put(int id)
{
    ssize_t len = BUFNUM;
    ssize_t read;
    if ((read =getline(&buffer[fill],&len, fp) )!=-1)
    {
        printf("producer %id:  put sth  from file in buffer\n", id);
        printf("\t\t\tsth:%s",buffer[fill]);
        fill = (fill + 1)%BUFNUM;
        return 0;
    }
    printf("the fill is end!!!!!, we can't put any thing in the buffer\n");
    return -1;
}

char* get()
{
    char *temp  = buffer[use];
    use = (use + 1)%BUFNUM;
    return temp;
}

void *producer(void *arg)
{
    int id = *(int *)arg;
    for (int i = 0; i < producerloops; i++)
    {
        sleep(1.2);
        sem_wait(&empty);
        sem_wait(&mutex);
        if (put(id)==0)
        {
            sem_post(&mutex);
            sem_post(&full);
        }
        else
        {
            // the file is end; so we can not produce any more
            sem_post(&mutex);

            // when in such situation, the only thing is that consumer
            //get the leftover things; producer do nothing is best; 
            // if producer do sth. it should be like nothing happend;
            sem_post(&empty);
        }
    }
}

void *consumer(void *arg)
{
    int id = *(int *)arg;
    for (int i = 0; i < consumerloops; i++)
    {
        sleep(0.5);
        sem_wait(&full);
        sem_wait(&mutex);
        char *tmp = get();
        printf("\t\tconsumer  %d: get ----- %s ",id,tmp);
        sem_post(&mutex);
        sem_post(&empty);
    }
}

int main()
{
    sem_init(&empty, 0, BUFNUM);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    fp = fopen("hello.txt","r");
    if (fp == NULL)
    {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i<BUFNUM; i++)
    {
        buffer[i] = (char*)malloc(20*sizeof(char));
    }


    pthread_t pConsumer[4];
    pthread_t pProducter[3];

    int i1 = 1, i2 = 2, i3 = 3, i4 = 4;
    pthread_create(&pConsumer[0], NULL, consumer, &i1);     
    pthread_create(&pConsumer[1], NULL, consumer, &i2);     
    pthread_create(&pConsumer[2], NULL, consumer, &i3);     
    pthread_create(&pConsumer[3], NULL, consumer, &i4);     

    pthread_create(&pProducter[0], NULL, producer, &i1);
    pthread_create(&pProducter[1], NULL, producer, &i2);
    pthread_create(&pProducter[2], NULL, producer, &i3);

    pthread_join(pConsumer[0], NULL);
    pthread_join(pConsumer[1], NULL);
    pthread_join(pConsumer[2], NULL);
    pthread_join(pConsumer[3], NULL);

    pthread_join(pProducter[0], NULL);
    pthread_join(pProducter[1], NULL);
    pthread_join(pProducter[2], NULL);

    for (int i = 0; i<BUFNUM; i++)
    {
        free(buffer[i]);
    }

    fclose(fp);

    printf("happy ending!!!!\n");
    // char *line = NULL;
}