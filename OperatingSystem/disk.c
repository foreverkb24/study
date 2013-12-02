/************************************************************
 * 操作系统实习报告                                         *
 * 磁盘存储空间的分配和回收                                 *
 * 2013-12-03                                               *
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#define COUNT 64

/*
 * 位示图，由盘组共8个柱面，每个柱面有2个磁道（盘面），
 * 每个磁道分成4个物理记录，需要64位来表示位示图，
 * filename记录块中保存的文件名 
 */
struct bitmapinfo { 
    int bitmap[COUNT]  ;
    char filename[COUNT][32];
};

struct bitmapinfo *bitinfo = NULL; 

/*
 * 打印位示图信息 
 */
void showbitmap(void) {
     //用来和位示图bitmap相与得到某位的状态 
     printf("当前系统磁盘位示图（0表示块可用）：\n");
     
     for(int i = 0 ;i<COUNT;i++) {    
		  if( i%8 == 0 ) {
              printf("\n");   
          } 
          printf("%d\t",bitinfo->bitmap[i]);
     }
	 printf("\n\n");   
}

/*
 * 分配物理块给 
 */
void diskallocate(void) {
     int neednum;    //由用户输入需要的物理块数：
     int busy = 0;
     char filestor[32]; 
     
     printf("输入需要分配的块数:\n");
     scanf("%d",&neednum);
     printf("输入文件名:\n");
     scanf("%s",filestor);
     for(int i=0;i<COUNT;i++) {
          busy =busy + bitinfo->bitmap[i]; 
     }
	 if( neednum > 64 - busy ) {
		 printf("没有足够的空闲磁盘，分配失败！\n");
     }
     else {
          printf("开始分配...\n"); 
          for(int i = 0 ; i<COUNT; i++)  {
			  if( bitinfo->bitmap[i] == 0 ) {
                  printf("分配块物理地址为:第%d个柱面，\t第%d个磁道,\t第%d个扇区\n",i/8+1,i%8/4,(i%8)%4);
                  bitinfo->bitmap[i] = 1;
                  strcpy(bitinfo->filename[i] , filestor);
                  neednum--;
              }
              if( 0==neednum) {
                  break;
              }
          }
          printf("分配成功！\n\n");   
     }
}

/*
 * 物理块回收
 */
void diskreturn(void) {
     char filereturn[32]; 
     printf("输入要回收的文件名: ");
     scanf("%s",filereturn);
     for(int i = 0 ; i<COUNT ; i++) {
		 if( strcmp(filereturn,bitinfo->filename[i]) != 0 ) {
            continue;     
         }
         else {
             bitinfo->bitmap[i] = 0; 
             printf("回收位示图中第%d字节的第%d位的物理块\n",i/8+1,i%8+1);
         }
     }
}


int main (void) {
     bitinfo = (struct bitmapinfo *)malloc(sizeof(struct bitmapinfo));
     for(int j = 0 ; j<COUNT ; j++) {
         bitinfo->bitmap[j]=0;
         memset( bitinfo->filename[j], 0, sizeof(char)*32 ); 
     }
    //功能选择 
    int func = 0;
     while( 1 ) {
           printf("*************首次适应算法实现主存分配和回收系统***********************\n");
           printf("--1:查看磁盘位示图\n");
           printf("--2:申请分配磁盘空间\n");
           printf("--3:申请回收磁盘空间\n");
           printf("--4:退出程序\n");
           printf("**********************************************************************\n");
           scanf("%d",&func);
           switch( func ) {
                   case 1 :showbitmap();break;
                   case 2 :diskallocate();break;
                   case 3 :diskreturn();break; 
                   case 4 :return 1;
           }
     }   
}
