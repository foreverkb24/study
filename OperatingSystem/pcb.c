/*
 * 操作系统第三次练习作业
 * 处理器调度：优先数调度算法
 * 日期：2013-11-19
 */
#include <stdlib.h>
#include <stdio.h>

#define STATE_READY 1;   //进程就绪状态
#define STATE_END 0;     //进程结束状态

/*
 * 进程结构
 */
struct PCB {
	char *name;		 //进程名
	struct PCB *next;  //指向下一个进程
	int runtime;     //进程运行时间
	int priority;    //进程优先级
	int state;       //进程状态
};

struct PCB *head = NULL;   //消息队列头

/*
 * 将一个进程加入到消息队列中
 */
void insert(struct PCB* p) {

	struct PCB* point = NULL;  //用来遍历消息队列的每一个位置
	struct PCB* insert = NULL;  //用来记录将p插入的位置
	if (head == NULL) {
		head = (struct PCB *)malloc(sizeof(struct PCB));
		head = p;
		head->next = NULL;
	}
	else { 
		point = head;
		while( p->priority < point->priority ) {
			insert = point;
			point = point->next;
			if( point == NULL) {
				break;
			}
		}
		if( point != head) {
			p->next = insert->next;
			insert->next = p;
		}
		else { 
			p->next = point;
			head = p;
		}
	}
}

/*
 * 打印消息队列中的进程信息
 */
void printPCB(void) {

	int id = 1;     //进程号
	struct PCB *point = head;
	printf("\t*******************消息队列****************\n");
	while ( point != NULL ) {
		printf("\t\t当前%d号进程为：%s\n",id,point->name);
		printf("\t\t当前%d号进程还需运行时间：%d\n",id,point->runtime);
		printf("\t\t当前%d号进程优先级为：%d\n",id,point->priority);
		printf("\t\t当前%d号进程为就绪状态!\n",id++);
		printf("\n");
		if( point->next != NULL) {
			point = point->next;
		}
		else {
			break;
		}
	}
	if( point == NULL) {
		printf("\t\t队列中已经没有进程！\n");
	}
	printf("\t***************消息队列打印完毕***************\n\n");
}

/*
 * 执行一次进程，队首进程运行时间减一，优先级减一
 * 若队首进程执行时间不为0,则将队首进程重新插入到
 * 队列中去
 */
void run(void) {
	
	struct PCB *point;
	printf("\t###########进程执行一次##############\n");
	if( head != NULL ) {
		printf("\t\t当前队首进程为:%s\n",head->name);
		head->runtime--;
		head->priority--;
		if ( head->runtime == 0 ) {
			printf("\t\t####%s执行完毕，退出队列!\n",head->name);
			head = head->next;
	}
		else {
			point = head;
			head = head->next;
			insert(point);
			printf("\t\t本次执行后，队首进程为:%s\n",head->name);
		}
		printf("\t#########本次进程执行结束#############\n");
		printPCB();
	}
	else {
		printf("\t#########队列中已经没有需要执行的进程##############\n"); 
	}
}			

/*
 *  初始化进程,并将进程加入到队列中去
 */
void initPCB(void) {
	struct PCB *p = malloc(sizeof(struct PCB));
	p->name  = "加法进程";
	p->runtime = 8;
	p->priority = 4;
	p->state = STATE_READY;
	insert(p);
	struct PCB *p2 = malloc(sizeof(struct PCB));
	p2->name = "减法进程";
	p2->runtime = 6;
	p2->priority = 3;
	p->state = STATE_READY;
	insert(p2);
	struct PCB *p3 = malloc(sizeof(struct PCB));
	p3->name = "乘法进程";
	p3->runtime = 4;
	p3->priority = 7;
	p->state = STATE_READY;
	insert(p3);
	struct PCB *p4 = malloc(sizeof(struct PCB));
	p4->name = "除法进程";
	p4->runtime = 3;
	p4->priority = 5;
	p->state = STATE_READY;
	insert(p4);
	struct PCB *p5 = malloc(sizeof(struct PCB));
	p5->name = "等于进程";
	p5->runtime = 2;
	p5->priority = 6;
	p->state = STATE_READY;
	insert(p5);

}
		
int main(void) {
	initPCB();
	printPCB();
	printf("请输入r来模拟一次进程执行!\n");
	while(1) {
		char r;
		scanf("%c",&r);
		if( r == 'r') {
			run();
		}
		if( head == NULL ) {
			break;
		}
	}
	free(head);
	return 0;
}
