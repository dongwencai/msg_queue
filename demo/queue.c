#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "semaphore.h"
#include "queue.h"

msg_queue_t *CreateMsgQueue(unsigned int msgMax,unsigned int msgSize)
{
    msg_queue_t *newQueue = NULL;
    newQueue = (msg_queue_t *)malloc(sizeof(msg_queue_t) + msgMax * msgSize);
    if(newQueue)
    {
        newQueue->msg_size=msgSize;
        newQueue->max_msg = msgMax;
        newQueue->read_off = newQueue->write_off = 0;
        pthread_mutex_init(&newQueue->lock,NULL);
        sem_init (&newQueue->sem, 0, 0); 
    }
    return newQueue;
}

int  DestoryMsgQueue(msg_queue_t *msgQueue)
{
    int ret = -1;
    if(msgQueue)
    {
        ret = sem_destroy(&msgQueue->sem);
        ret |= pthread_mutex_destroy(&msgQueue->lock);
        free(msgQueue);
    }
    return ret;
}

int SendMsg(msg_queue_t *msgQueue,void *msg)
{
    int ret = -1;
    pthread_mutex_lock(&msgQueue->lock);

    if((msgQueue->write_off + 1) % msgQueue->max_msg == msgQueue->read_off)
    {
        pthread_mutex_unlock(&msgQueue->lock);
        fprintf(stderr,"message queue is full\n");
        return ret;
    }

    memcpy(&msgQueue->msg[msgQueue->write_off * msgQueue->msg_size],msg,msgQueue->msg_size);
    msgQueue->write_off ++;
    if(msgQueue->write_off >= msgQueue->max_msg)
    {
        msgQueue->write_off = 0;
    }
    ret = sem_post (&msgQueue->sem); 
    pthread_mutex_unlock(&msgQueue->lock);
    return ret;
}

int RecvMsg(msg_queue_t *msgQueue,void *msg,int blocked)
{
    int ret = 0;
    if(blocked)
    {
        sem_wait(&msgQueue->sem);
    }
    else
    {
        sem_trywait(&msgQueue->sem);
    }
    pthread_mutex_lock (&msgQueue->lock); 
    if (msgQueue->read_off != msgQueue->write_off) 
    { 
        memcpy(msg,msgQueue->msg + msgQueue->read_off * msgQueue->msg_size,msgQueue->msg_size); 
        ++ msgQueue->read_off; 
        if (msgQueue->read_off >= msgQueue->max_msg) 
            msgQueue->read_off = 0; 
    }
    else
    {
        pthread_mutex_unlock (&msgQueue->lock);
        fprintf(stderr,"message queue is empty\n");
        printf("line:%d\tread offset:%d\t write offset:%d\n",__LINE__,msgQueue->read_off,msgQueue->write_off);
        exit(-1);
        ret = -1;
    }	
    pthread_mutex_unlock (&msgQueue->lock);
    return ret; 
} 

