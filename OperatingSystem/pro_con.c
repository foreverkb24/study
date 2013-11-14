#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define PRO_NUM 5 //生产者数目
#define CON_NUM 6  //消费者数目
#define BUFFER 10   //缓冲区大小

int pro = 0;   //生产者生产产品的产品号
int con = 0;   //消费者消费产品的产品号
int buffer[BUFFER] = {0,0,0,0,0,0,0,0,0,0};  //初始化缓冲区，0代表当前位置没有产品，如果有产品则显示为产品号

sem_t empty;    //同步信号量，当缓冲区满时不让生产者继续向缓冲区添加
sem_t full;     //同步信号两，当没产品时消费者无法从缓冲区取出
pthread_mutex_t mutex;  //互斥信号量，一次只有一个进程访问缓冲区
int production_id = 0;   //生产者id
int consumer_id = 0;   //消费者id

/*
 * 打印缓冲区
 */
void print() {
	int i;
	for(i = 0; i < BUFFER ; i++) 
		if(i != BUFFER-1) {
			printf("%d---",buffer[i]);
		}
		else {
			printf("%d",buffer[i]);
		}
	printf("\n");
}

/*
 * 生产者方法
 */
void *product() {
	int id = ++production_id;

	while(1) {
		sleep(1);

		sem_wait(&empty);        //empty-1,如果小于0则进程等待，否则执行下面程序
								 //生成一个产品并放入缓冲区
		pthread_mutex_lock(&mutex); //为当前进程枷锁，保证不会有其他进程同时访问缓冲区

		printf("生产者%d生产了第%d号产品!   ",id,pro);

		buffer[pro%BUFFER] = pro + 1;
		print();
		++pro;

		pthread_mutex_unlock(&mutex);  
		sem_post(&full);  //full+1,如果大于0，消费者进程可以被激活
	}
}

/*
 * 消费者方法
 */
void *consume() {
	int id = ++consumer_id;
	
	while(1) {
		sleep(1);

		sem_wait(&full);   //full-1,如果小于0则进程等待，否则执行下面程序消费
						   //一个产品，并且从缓冲区移除
		pthread_mutex_lock(&mutex);

		printf("<<<消费者%d消费了第%d号产品!   ",id,con);

		buffer[con%BUFFER] = 0;
		print();
		++con;

		pthread_mutex_unlock(&mutex);
		sem_post(&empty);    //empty+1,如果大于0则生产者进程可以被激活
	}
}

int main() {
	pthread_t productor[PRO_NUM];
	pthread_t consumer[CON_NUM];
	int i;  //因为gcc编译基于c89标准，c89标准不支持for循环内定义变量

	//初始化同步信号量 
	int init_empty = sem_init(&empty ,0,BUFFER);
	int init_full = sem_init(&full,0,0);

	if( init_empty && init_full != 0) {
		printf("sem init failed \n");
		exit(1);
	}

	//初始化互斥信号量
	int init_mutex = pthread_mutex_init(&mutex,NULL);
	if( init_mutex != 0) {
		printf("mutex init failed \n");
		exit(1);
	}

	//创建PRO_NUM个生产者线程
	for( i=0; i<PRO_NUM ; i++) {
		int pid = pthread_create(&productor[i],NULL,product,(void *)(&i));
		if(pid !=0 ) {
			printf("product%d creation failed \n",1);
			exit(1);
		}
	}

	//创建CON_NUM个消费者线程
	for( i=0; i<CON_NUM; i++) {
		int pid = pthread_create(&productor[i],NULL,consume,(void *)(&i));
		if(pid !=0 ) {
			printf("consumer%d creation failed \n",1);
			exit(1);
		}
	}

	//销毁生产者线程
	for( i=0;i<PRO_NUM; i++) {
		pthread_join(productor[i],NULL);
	}

	//销毁消费者线程
	for( i=0; i<CON_NUM;i++) {
		pthread_join(consumer[i],NULL);
	}
	exit(0);
}




