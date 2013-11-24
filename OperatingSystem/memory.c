/******************************************************************
 *  操作系统实验报告                                              *
 *  主存空间的分配和回收:采用首次适应算法实现                     *
 *  2013-11-24													  *
 ******************************************************************/

#include <stdio.h>
#include<malloc.h>

struct FBT {
	int startdress;			//分区首地址
	int size;					//分区所占内存大小
	int processid;			
//所申请内存分区的进程号，本来FBT中没有这一项，但是便于内存的回收我加了这一项
	int state;				//此分区的状态
	struct FBT *next;			//指针，指向下一个结点（分区）
};

struct FBT *create() {		//建立自由块表
	struct FBT *head,*p,*q;	
	head=NULL;
	p=(struct FBT *)malloc (sizeof(struct FBT));//申请一个新的结点，用来存放新的分区
	q=(struct FBT *)malloc (sizeof(struct FBT));//申请一个新的结点，用来存放新的分区
	head=p;
	p->size =5;						//系统分区，大小为5
	p->processid =-1;					//－1表示操作系统的固定分区
	p->startdress =0;					//开始地址为0
	p->state =1;						//状态为已分配，值为1
	p->next =q;						//指向下一个结点
	q->size=123;						//余下的分区大小为123
	q->processid=0;					//所对应的进程号为0，表示都未分配
	q->startdress=5;					//开始地址为5
	q->state=0;						//状态为未分配，值为0
	q->next=NULL;						//指向下一个分区，因为只有两个分区，所以为空
	return head;
}

struct FBT *distribute(struct FBT *head) {	//分配内存子函数
	int id,neededsize;
	struct FBT *pretail,*tail,*p;
	printf("请输入一个进程ID(数字):");	//输入要申请内存的进程号
	scanf("%d",&id);
	printf("请输入所需内存大小:");	//输入要申请的内存大小
	scanf("%d",&neededsize);
	pretail=tail=head;

	while (tail!=NULL) {
		if(tail->state ==0&&tail->size >=neededsize) {
//如果此分区没有分配，而且大小大于等于申请的内存大小
			if(tail->size >neededsize) {
//如果此分区大小大于要申请的大小，分配，并把余下的再分割成一个分区
				p=(struct FBT *)malloc (sizeof(struct FBT));
				p->size =neededsize;
				p->processid =id;
				p->startdress =tail->startdress ;
				p->state =1;
				if(head!=tail) {
					pretail->next =p;
					p->next =tail;
					pretail=p;
				}
				else {
					p->next =tail;
					head=p;	
					tail->next =NULL;
				}
				tail->startdress =p->startdress+p->size ;
				tail->size =tail->size -neededsize;
				break;
			}
			if(tail->size =neededsize) { //如果此分区等于要申请的大小，直接分配即可
				tail->processid =id;
				tail->state =1;
				break;
			}
		}    				
		else {			//否则，指向下一个结点继续判断
			pretail=tail;
			tail=tail->next ;
		}
	}
	if(tail==NULL)	//如果遍历完链表都没有找到合适的分区分配，则显示以下信息
		printf("\n对不起，没有足够大的存储空间分配!\n");
	return head;		//返回头指针
}

struct FBT *callback(struct FBT *head) {	//回收内存子函数
	int id;
	struct FBT *pretail,*tail;
	printf("请输入将要回收的进程ID（数字):");
	scanf("%d",&id);
	pretail=tail=head;
	while(tail!=NULL) {
		//遍历链表
		if(tail->processid ==id) {
		//如果改分区所对应的进程号为要收回分区的进程号，则做以下动作
			if(tail->next !=NULL) {    
				//如果改分区不是最后一个分区，则做以下动作
				if(pretail->state ==1&&tail->next->state ==1 ) {
				//前一个分区是已分配的且后一个分区也是已分配的
					tail->state =0;
					break;
				}
				if(pretail->state ==0&&tail->next->state ==1 ) {
				//前一个分区是未分配的且后一个分区是已分配的
					pretail->next =tail->next;
					pretail->size =tail->size +pretail->size ;
					free(tail);
					break;
				}
				if(pretail->state ==1&&tail->next->state ==0 ) {
				//前一个分区是已分配的且后一个分区是未分配的
					if(pretail!=tail) {
						pretail->next =tail->next ;
						tail->next ->size =tail->next ->size +tail->size;
						tail->next ->startdress =tail->startdress ;
						free(tail);
						break;
					}
					else  {
						head=tail->next ;
						tail->next->startdress =0;
						tail->next->size +=tail->size;
						break;
					}
				}
				if(pretail->state ==0&&tail->next->state ==0 ) {
				//前一个分区和后一个分区都是未分配的
					pretail->next =tail->next->next ;
					pretail->size =pretail->size +tail->size +tail->next ->size ;
					free(tail->next);
					free(tail);
					break;
				}
			}
			else {				 //如果改分区是最后一个分区则做以下动作
				if(pretail->state ==1)	//如果前一个分区是已分配的
				{
					tail->state =0;
					break;
				}
				else {			//如果前一个分区是未分配的
					pretail->next =NULL;
					pretail->size=pretail->size +tail->size ;
					free(tail);
					break;
				}
			}
		}
		pretail=tail;
		tail=tail->next ;			//遍历下一个分区结点
	}
return head;						//返回头指针
}


void print(struct FBT *head) {		//显示FBT的内容给用户
	struct FBT *tail=head;
	printf("起始地址\t空间大小\t使用状态\t\t进程ID\t\n");	
	tail=head;
	while(tail!=NULL) {
		if(tail->state==1)
			printf("%5d\t\t%5d\t\t%5d\t\t\t%5d\n",tail->startdress ,tail->size ,tail->state ,tail->processid );
		else
			printf("%5d\t\t%5d\t\t%5d\n",tail->startdress ,tail->size ,tail->state );	
		tail=tail->next ;
	}
}

void main() {
	int choice=1;
	struct FBT *head;
	head=create();
	while(choice!=0) {		//显示选择菜单，供用户选择
		printf("\t1.分配内存空间\n");
		printf("\t2.回收内存空间\n");
		printf("\t0.退出\n");
		printf("\t请选择:");
		scanf("%d",&choice);
		switch (choice) {
			case 1:head=distribute(head);
				   print(head);
				   break;  //选择1，则做内存分配
			case 2:head=callback(head);
				   print(head);
				   break;  //选择2，则做内存回收
			case 0:	break; 
			default :printf("输入错误!\n");			//否则显示选择错误
		}
	}
}

