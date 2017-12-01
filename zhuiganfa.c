/*************************************************************************
	> File Name: zhuiganfa.c
	> Author: Zhu Brocadesoar
	> Mail: brocadesoar@gmail.com
	> Created Time: Mon 27 Nov 2017 10:34:17 PM CST
 ************************************************************************/

#include<stdio.h>
#include<math.h>
#define JIESHUMAX 1000

/*
 * 对于 三对角方程 AX=f
 * A = 
 * 
 *  b1 c1 
 *  a2 b2 c2
 *     a3 b3 c3
 *
 *              an-1 bn-1 cn-1
 *                  an    bn
 *
 * X = 
 *
 *  x1
 *  x2
 *  x3
 *
 *  xn
 *
 * f =
 *
 *  f1
 *  f2
 *  f3
 *
 *  fn
 *
 * 该函数的输入参数表示：
 * Input arguments(pointer of array):
 *
 * jieshu = n
 * va = {0, a2, a3...an}
 * vb = {b1, b2, b3...bn}
 * vc = {c1, c2, c3...cn-1, 0}
 * vx = {x1, x2, x3...xn}
 * vf = {f1, f2, f3...fn}
 *
 * 返回值:
 * Return values:
 *
 * 0 Success
 * 1 |b1| > |c1| > 0 Invalid
 * 2 |bi| >= |ai| + |ci| and ai * ci != 0 Invalid
 * 3 |bn| > |an| > 0 Invalid
 *
* */

int zhuiganfa(int jieshu, 
               double * va, 
               double * vb, 
               double * vc, 
               double * vx, 
               double * vf){
                   // 参数检查 check arguments
                   if(!( fabs( vb[1 - 1] ) > fabs( vc[1 - 1] ) 
                       && fabs( vc[1 - 1] ) > 0 )){
                           // |b1| > |c1| > 0 Invalid
                           return 1;
                   }else if(!( fabs( vb[jieshu - 1] ) > fabs( va[jieshu - 1] )
                              && fabs( va[jieshu - 1] ) > 0)){
                                  // |bn| > |an| > 0 Invalid
                                  return 3;
                              }
                   int flag = 0;
                   for(int i = 2; i <= jieshu - 1; i++){
                       if(!( fabs( vb[i - 1] ) >= ( fabs( va[i - 1] ) + fabs( vc[i - 1] ) )
                            && ( va[i - 1] * vc[i - 1] != 0 ))){
                                // |bi| >= |ai| + |ci| and ai * ci != 0 Invalid
                                return 2;
                            }
                   }
                   // 追过程 process 1，vbeta 
                   double vbeta[JIESHUMAX];
                   int i = 1;
                   vbeta[1 - 1] = vc[1 - 1] / vb[1 - 1]; 
                   for(i = 2; i <= jieshu; i++){
                       vbeta[i - 1] = vc[i - 1] / (vb[i - 1] - va[i - 1] * vbeta[i - 1 - 1]);
                   }
                   // 追过程 process 2，vy
                   double vy[JIESHUMAX];
                   i = 1;
                   vy[1 - 1] = vf[1 - 1] / vb[1 - 1];
                   for(i = 2; i <= jieshu; i++){
                       vy[i - 1] = (vf[i - 1] - va[i - 1] * vy[i - 1 - 1]) / (vb[i - 1] - va[i - 1] * vbeta[i - 1 - 1]);
                   }
                   // 赶过程 process 3 , vx
                   i = jieshu;
                   vx[jieshu - 1] = vy[jieshu - 1]; 
                   for(i = jieshu - 1; i >= 1; i--){
                       vx[i - 1] = vy[i - 1] - vbeta[i - 1] * (vx[i + 1 - 1]);
                   }
                   return 0;
}

int main(void){
    // 输入参数
    double a[4] = {0, 1.0/2, 1.0/2, 1.0};
    double b[4] = {2.0, 2.0, 2.0, 2.0};
    double c[4] = {1.0, 1.0/2, 1.0/2, 0};
    double x[4];
    double f[4] = {-0.5, 0, 0, 0};
    if(zhuiganfa(4, a, b, c, x, f)){
        puts("Error");
    }else{
        for(int i = 0; i < 4; i++){
            printf("x[%d] = %lf\t", i + 1, x[i]);
        }
    }
    puts("");

    return 0;
}

