/*************************************************************************
	> File Name: qain.c
	> Author: Zhu Brocadesoar
	> Mail: brocadesoar@gmail.com
	> Created Time: Thu 21 Sep 2017 09:07:16 AM CST
 ************************************************************************/

#include<stdio.h>

#define Nmax 5 // 总层数
#define N (Nmax - 1)
#define START 0 // 开始值

int pos[N + 1];

void fun(int src, int lay){
    pos[lay]++;
    printf("lay = %d\t\tpos = %d\t\tsrc = %d\n", lay, pos[lay], src);
    if(lay > N){
        return ;
    }
    fun(src + 1, lay + 1);
    fun(src - 1, lay + 1);
    return ;
}

int main(void){
    printf("总层数5，顶层开始值为0\n");
    puts("lay 第几层， pos 第几位， src 结果值");
    for(int i = 0; i < N + 1; i++){
        pos[i] = 0;
    }
    fun(START, 1);
    return 0;
}

/* 运行结果:


总层数5，顶层开始值为0
lay 第几层， pos 第几位， src 结果值 :
lay = 1		pos = 1		src = 0
lay = 2		pos = 1		src = 1
lay = 3		pos = 1		src = 2
lay = 4		pos = 1		src = 3
lay = 5		pos = 1		src = 4
lay = 5		pos = 2		src = 2
lay = 4		pos = 2		src = 1
lay = 5		pos = 3		src = 2
lay = 5		pos = 4		src = 0
lay = 3		pos = 2		src = 0
lay = 4		pos = 3		src = 1
lay = 5		pos = 5		src = 2
lay = 5		pos = 6		src = 0
lay = 4		pos = 4		src = -1
lay = 5		pos = 7		src = 0
lay = 5		pos = 8		src = -2
lay = 2		pos = 2		src = -1
lay = 3		pos = 3		src = 0
lay = 4		pos = 5		src = 1
lay = 5		pos = 9		src = 2
lay = 5		pos = 10		src = 0
lay = 4		pos = 6		src = -1
lay = 5		pos = 11		src = 0
lay = 5		pos = 12		src = -2
lay = 3		pos = 4		src = -2
lay = 4		pos = 7		src = -1
lay = 5		pos = 13		src = 0
lay = 5		pos = 14		src = -2
lay = 4		pos = 8		src = -3
lay = 5		pos = 15		src = -2
lay = 5		pos = 16		src = -4


*/
