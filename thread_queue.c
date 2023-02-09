/*
 * @Description: thread_queue library
 * @Version: 1.0
 * @Author: lanmeng
 * @Date: 2022-10-20 11:32:03
 * @LastEditTime: 2022-11-04 10:10:28
 * @LastEditors: lanmeng
 */

#include "thread_queue.h"

#include <pthread.h>

struct QueueNode
{
	struct QueueNode * next;
};

struct LQueue
{
	struct QueueNode pHeader;
	int m_size;
	struct QueueNode * pTail;
	pthread_mutex_t m_lock;
	pthread_cond_t m_cond;
};

LinkQueue init_LinkQueue()
{
	struct LQueue * myQueue = malloc(sizeof(struct  LQueue));
	if (myQueue == NULL)
	{
		LOG_ERR("malloc error");
		return NULL;
	}

	myQueue->pHeader.next = NULL;
	myQueue->m_size = 0;
	myQueue->pTail = &myQueue->pHeader;

	pthread_mutex_init(&myQueue->m_lock, NULL);		
	pthread_cond_init(&myQueue->m_cond, NULL);

	return myQueue;
}

void push_LinkQueue(LinkQueue queue, void * data)
{
	if (queue == NULL) 
	{
		LOG_ERR("Invalid queue !");
		return;
	}
	if ( data == NULL)
	{
		LOG_ERR("Invalid data !");
		return;
	}

	struct LQueue * myQueue = queue;
	struct QueueNode * myNode = data;

	pthread_mutex_lock(&myQueue->m_lock);
	myQueue->pTail->next = myNode;
	myNode->next = NULL;
	myQueue->pTail = myNode;
	myQueue->m_size++;
	pthread_mutex_unlock(&myQueue->m_lock);
}

void pop_LinkQueue(LinkQueue queue)
{
	if (queue == NULL) 
	{
		LOG_ERR("Invalid queue !");
		return;
	}
	struct LQueue * myQueue = queue;
	
	if (myQueue->m_size == 0) return;

	pthread_mutex_lock(&myQueue->m_lock);

	if (myQueue->m_size == 1)
	{
		myQueue->pHeader.next = NULL;
		myQueue->pTail = &myQueue->pHeader;
		myQueue->m_size--;
		pthread_mutex_unlock(&myQueue->m_lock);
		return;
	}

	struct QueueNode * pFirst = myQueue->pHeader.next;

	myQueue->pHeader.next = pFirst->next;
	myQueue->m_size--;
	
	pthread_mutex_unlock(&myQueue->m_lock);
}

int size_LinkQueue(LinkQueue queue)
{
	if (queue == NULL) 
	{
		LOG_ERR("Invalid queue !");
		return -1;
	}
	struct LQueue * myQueue = queue;
	return myQueue->m_size;
}

int isEmpty_LinkQueue(LinkQueue queue)
{
	if (queue == NULL) 
	{
		LOG_ERR("Invalid queue !");
		return -1;
	}
	struct LQueue * myQueue = queue;
	if (myQueue->m_size == 0) return 1;
	
	return 0;
}

void * front_LinkQueue(LinkQueue queue)
{
	if (queue == NULL) 
	{
		LOG_ERR("Invalid queue !");
		return NULL;
	}
	struct LQueue * myQueue = queue;

	return myQueue->pHeader.next;
}


void * back_LinkQueue(LinkQueue queue)
{
	if (queue == NULL) 
	{
		LOG_ERR("Invalid queue !");
		return NULL;
	}
	struct LQueue * myQueue = queue;
	return myQueue->pTail;
}


void destroy_LinkQueue(LinkQueue queue)
{
	if (queue == NULL) 
	{
		LOG_ERR("Invalid queue !");
		return;
	}
	struct LQueue * myQueue = queue;

	while(!isEmpty_LinkQueue(myQueue))
	{
		pop_LinkQueue(myQueue);
	}

	pthread_mutex_destroy(&myQueue->m_lock);
	pthread_cond_destroy(&myQueue->m_cond);
	free(queue);
	queue = NULL;
}

#ifdef DEBUG_THREAD_QUEUE_TEST
#include <unistd.h>

/**
 * @brief : This function is used to set the recorder's single-thread test.
 */
#define MAX_TEST_SIZE   10

typedef struct Data {
    void* node;
    int interal_data;
} Data;

void threadQueueTest() {
    printf("%s\n", __func__);

    LinkQueue queue = init_LinkQueue();
    Data* data[MAX_TEST_SIZE] = { 0 };
    for (size_t i = 0; i < MAX_TEST_SIZE; i++)
    {
        data[i] = (Data*)malloc(sizeof(Data));
        // *data[i] = i;
        data[i]->interal_data = i + 1;
        push_LinkQueue(queue, data[i]);
        printf("insert data is %d, queue size is %d\n", data[i]->interal_data, 
                size_LinkQueue(queue));
    }
    
    for (size_t i = 0; i < MAX_TEST_SIZE; i++)
    {
        Data *tmp = front_LinkQueue(queue);
        pop_LinkQueue(queue);
        printf("%d, queue size is %d\n", tmp->interal_data, size_LinkQueue(queue));
        free(tmp);
        tmp = NULL;
    }

    destroy_LinkQueue(queue);
}

/**
 * @brief : This function is used to set the multithreading test of the recorder.
 */
int internalThreadQueueMultThreadTest();

void threadQueueMultThreadTest() {
    printf("%s\n", __func__);
    internalThreadQueueMultThreadTest();
}

struct Node
{
	void *node;
	int number;
	struct Node *next;
};

struct Person {
	void * node;
	char name[64];
	int age;
};

struct uart_buf {
	void * node;
	char *buf;
	int length;
};

pthread_cond_t cond;
pthread_mutex_t mutex;
struct Node *head = NULL;
static int test_age = 0;
static int status = 1;

void * producter(void* arg)
{
	LinkQueue *queue = (LinkQueue *)arg;
	while (1)
	{
		pthread_mutex_lock(&mutex);
		struct Person *new_person = (struct Person *)malloc(sizeof(struct Person));
		if(new_person == NULL) {
			LOG_ERR("new_person malloc error");
			break;
		}
		new_person->node = NULL;
		strcpy(new_person->name, "new_person");
		new_person->age = ++test_age;
		push_LinkQueue(queue, new_person);
		printf("producter name:[ %s ], age = %d, tid = %ld\t", 
				 new_person->name, new_person->age, pthread_self());
		struct Person * pFront = front_LinkQueue(queue);
		printf("front name:%s  age: %d\t", pFront->name, pFront->age);
		struct Person * pBack = back_LinkQueue(queue);
		printf("tail name:%s  age: %d\t", pBack->name, pBack->age);
		printf("queue size: %d\n", size_LinkQueue(queue));
		
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 3);
	}

	return NULL;
}

void * consumer(void* arg)
{
	LinkQueue *queue = (LinkQueue *)arg;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while (size_LinkQueue(queue) == 1) {
			pthread_cond_wait(&cond, &mutex);
		}

		struct Person *new_person = front_LinkQueue(queue);
		printf("consumer  name:[ %s ], age = %d, tid = %ld\t", 
				 new_person->name, new_person->age, pthread_self());
		printf("front name:%s  age: %d \t", new_person->name, new_person->age);
		pop_LinkQueue(queue);
		free(new_person);
		new_person = NULL;

		struct Person * pBack = back_LinkQueue(queue);
		printf("tail name:%s  age: %d\t", pBack->name, pBack->age);
		printf("queue size: %d\n", size_LinkQueue(queue));
		pthread_mutex_unlock(&mutex);

		sleep(rand() % 3);
	}

	return NULL;
}

int internalThreadQueueMultThreadTest()
{
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);

	LinkQueue myQueue = init_LinkQueue();
	printf("queue size is %d\n", size_LinkQueue(myQueue));
	pthread_t thread_p[5], thread_c[5];

	for (size_t i = 0; i < 5; i++) {
		pthread_create(&thread_p[i], NULL, producter, myQueue);
	}

	for (size_t i = 0; i < 5; i++) {
		pthread_create(&thread_c[i], NULL, consumer, myQueue);
	}

	for (size_t i = 0; i < 5; i++) {
		pthread_join(thread_p[i], NULL);
		pthread_join(thread_c[i], NULL);
	}

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
	
	return 0;
}

#endif // !DEBUG_CLOG_TEST
