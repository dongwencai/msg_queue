#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "queue.h"

typedef struct{
    pthread_t thread;
    int     rand_number;
}msg_t;

void *product(void *param)
{
    msg_queue_t *msgQueue=(msg_queue_t *)param;
    msg_t msg;
    msg.thread = pthread_self();
    while(1)
    {
        msg.rand_number = rand() % 100;
        if(SendMsg(msgQueue,&msg) < 0)
        {
            usleep(100);
        }
        //usleep(1000);
    }
}
void *customer(void *param)
{
    msg_queue_t *msgQueue = (msg_queue_t *)param;
    msg_t msg;
    while(1)
    {
        RecvMsg(msgQueue,&msg,1);
        printf("thread:%u\t rand sum:%d\n",msg.thread,msg.rand_number);
    }
}
int main()
{
    int cnt;
    msg_t msg;
    msg_queue_t *msgQueue = NULL;
    pthread_t thread1,thread2,thread3,thread4,thread5,thread6,thread7,thread8;
    msg.thread = 0;
    if(msgQueue = CreateMsgQueue(100,sizeof(msg_t)))
    {
        pthread_create(&thread1,NULL,product,msgQueue);
        pthread_create(&thread2,NULL,product,msgQueue);
        pthread_create(&thread3,NULL,product,msgQueue);
        pthread_create(&thread6,NULL,product,msgQueue);
        pthread_create(&thread7,NULL,product,msgQueue);
        pthread_create(&thread8,NULL,product,msgQueue);
        pthread_create(&thread4,NULL,customer,msgQueue);
        pthread_create(&thread5,NULL,customer,msgQueue);
        pthread_join(thread1,NULL);
        pthread_join(thread2,NULL);
        pthread_join(thread3,NULL);
        pthread_join(thread6,NULL);
        pthread_join(thread7,NULL);
        pthread_join(thread8,NULL);
        pthread_join(thread4,NULL);
        pthread_join(thread5,NULL);
        DestoryMsgQueue(msgQueue);
    }
    return 0;
}
