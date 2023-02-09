## 一个易用的线程安全队列

此队列数据结构是线程安全的，并且效率极高（极少的数据拷贝）

包含简单易用的操作队列的方法以及两个测试用例

单线程的队列测试较为简单。只有队列的创建，入队，出队，销毁队列（释放资源）等简单操作。

多线程的队列测试设计较为复杂。

多线程测试用例中包含了生产者消费者模型。

构建了一个边生产边数据，边消费数据。使得队列内容以及排队数量不大，并且通过终端易于观察数据。

## 操作队列所用的库

```c
/* Simple and easy-to-use method for operating queues */

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
```

## 测试用例分析

```c
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
```

## 在多线程测试用例里运用生产者消费者模型去构建边产生数据边消耗数据的方式测试线程安全队列

### 生产者设计

```c
/* producter design */
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
```

### 消费者设计

```c
/* consumer design */
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

		// 访问队尾
		struct Person * pBack = back_LinkQueue(queue);
		printf("tail name:%s  age: %d\t", pBack->name, pBack->age);
		printf("queue size: %d\n", size_LinkQueue(queue));
		pthread_mutex_unlock(&mutex);

		sleep(rand() % 3);
	}

	return NULL;
}
```

### 多线程测试用例的构建

```c
/* multithreading test case design */
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
```

