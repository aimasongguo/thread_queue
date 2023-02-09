/*
 * File name   : test.c
 * Author      : lanmeng
 * Date        : Created on 2022/11/19.
 * Description : 
 * Others      : No supplementary explanation for the time being.
 */

#include "thread_queue.h"
#include "test.h"
#include <stdio.h>

#define THREAD_QUEUE_TEST_THREAD

void test() {
#ifndef THREAD_QUEUE_TEST_THREAD
    printf("thread queue test no thread\n");
    threadQueueTest();
#else
    printf("thread queue test thread\n");
    threadQueueMultThreadTest();
#endif
}
 