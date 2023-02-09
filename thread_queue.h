/*
 * @Description: thread_queue library
 * @Version: 1.0
 * @Author: lanmeng
 * @Date: 2022-10-20 11:32:03
 * @LastEditTime: 2022-11-04 10:10:28
 * @LastEditors: lanmeng
 */
#ifndef _THREAD_QUEUE_H_
#define _THREAD_QUEUE_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LOG_ERR(fmt, ...) do{\
	printf("[ERROR]  "fmt"  [line:%d] [%s]\n", ##__VA_ARGS__, __LINE__, __FUNCTION__);\
}while(0);
 
#define LOG_WARN(fmt, ...) do{\
	printf("[WARNING]  "fmt"  [line:%d] [%s]\n", ##__VA_ARGS__, __LINE__, __FUNCTION__);\
}while(0);


#define DEBUG_THREAD_QUEUE_TEST

/**
 * @brief : Contains a pointer to the instance of the LinkQueue library.
 */
typedef void * LinkQueue;

/**
 * @description: This function is used to instantiate an object of type LinkQueue.
 * @return {*}	contains a pointer to the instance of the LinkQueue library.
 */
LinkQueue init_LinkQueue();

/**
 * @description: This function is used to queue data.
 * @return {*} : No return value
 * @param {LinkQueue} queue : contains a pointer to the instance of the LinkQueue 
 *               library.
 * @param {void *} data : Data that needs to be queued.
 */
void push_LinkQueue(LinkQueue queue , void * data);

/**
 * @description: This function pops up the queue header element.
 * @return {*} : No return value
 * @param {LinkQueue} queue : contains a pointer to the instance of the LinkQueue 
 *               library.
 */
void pop_LinkQueue(LinkQueue queue);

/**
 * @description: This function is used to get the size of the specified queue.
 * @return {*} : Specify the size of the queue.
 * @param {LinkQueue} queue : contains a pointer to the instance of the LinkQueue 
 *               library.
 */
int size_LinkQueue(LinkQueue queue);

/**
 * @description: This function is used to determine whether there is data in the 
 *               specified queue.
 * @return {*} : -1 -> queue is NULL, error.
 *                0 -> queue is no empty.
 *                1 -> queue is empty.
 * @param {LinkQueue} queue : contains a pointer to the instance of the LinkQueue 
 *               library.
 */
int isEmpty_LinkQueue(LinkQueue queue);

/**
 * @description: This function is used to access the data at the specified queue 
 *               head.
 * @return {*} : Returns a pointer to the address of the data at the head of the 
 *               specified queue.
 *               NULL -> queue is NULL, error.
 * @param {LinkQueue} queue : contains a pointer to the instance of the LinkQueue 
 *               library.
 */
void * front_LinkQueue(LinkQueue queue);

/**
 * @description: This function is used to access the data at the end of the 
 *               specified queue.
 * @return {*} : Returns a pointer to the data address at the end of the 
 *               specified queue.
 *               NULL -> queue is NULL, error.
 * @param {LinkQueue} queue : contains a pointer to the instance of the LinkQueue
 *               library.
 */
void * back_LinkQueue(LinkQueue queue);

/**
 * @description: Release the memory space requested when creating the specified 
 *               queue.
 * @return {*} : No return value
 * @param {LinkQueue} queue : contains a pointer to the instance of the LinkQueue 
 *               library.
 */
void destroy_LinkQueue(LinkQueue queue);

/* test case */
#ifdef DEBUG_THREAD_QUEUE_TEST
/**
 * @brief : This function is used to set the recorder's single-thread test.
 */
void threadQueueTest();

/**
 * @brief : This function is used to set the multithreading test of the recorder.
 */
void threadQueueMultThreadTest();
#endif // !DEBUG_CLOG_TEST

#endif // !_THREAD_QUEUE_H_
