/*************************************************************************
	> File Name: zuixiaoercheng.c
	> Author: Zhu Brocadesoar
	> Mail: brocadesoar@gmail.com
	> Created Time: Fri 01 Dec 2017 01:48:12 PM CST
 ************************************************************************/

#include<stdio.h>
#include<math.h>
#define JIESHUMAX 1000

/*打印二维数组，测试用*/
void display2(int jieshu, void * pA){
    double (*A)[jieshu];
    A = (double (*)[])pA;
    for(int i = 1; i <= jieshu ; i++){
        for(int j = 1; j <= jieshu; j++){
            printf("%lf\t", A[i - 1][j - 1]);
        }
        printf("\n");
    }
    printf("----------------\n");
}

/*
 * LU分解法
 * 对于方程 Ax=b
 *
 * A = 
 *
 *  a1,1 a1,2 ... a1,j ... a1,n
 *
 *  a2,1 a2,2 ... a2,j ... a2,n
 *  
 *   :    :        :        :
 *
 *  ai,1 ai,2 ... ai,j ... ai,n
 *
 *   :    :        :        :
 *
 *  an,1 an,2 ... an,j ... an,n
 *
 * x = 
 *
 *  x1
 *
 *  x2
 *
 *   :
 *
 *  xn
 *
 * b =
 *
 *  b1
 *
 *  b2
 *
 *   :
 *
 *  bn
* */

// 返回值非0值则方程奇异
int lufenjiefa(const int jieshu, // 阶数
               void * pA, // 二维数组名转换为空型指针
               double * x, // 一维数组名 解 
               double * b){ // 一维数组名
    double (*A)[jieshu];
    A = (double (*)[])pA;
    double L[jieshu][jieshu];
    double U[jieshu][jieshu];
    // U第一行 1st row of U
    for(int j = 1; j <= jieshu; j++){
        U[1 - 1][j - 1] = A[1 - 1][j - 1];
        if(U[1 - 1][1 - 1] == 0) return 1;
    }
    // L第一列 1st column of L
    for(int i = 1; i <= jieshu; i++){
        L[i - 1][1 - 1] = A[i - 1][1 - 1] / U[1 - 1][1 - 1];
    }
    // U 第r行 rth row of U 和 L 第r列 rth column of L
    for(int r = 2; r <= jieshu; r++){
        // U 第r行第j列 rth row, jth column of U
        for(int j = r; j <= jieshu; j++){
            double sum = 0;
            for(int k = 1; k <= r - 1; k++){
                sum += L[r - 1][k - 1] * U[k - 1][j - 1];
            }
            U[r - 1][j - 1] = A[r - 1][j - 1] - sum;
            if(U[r - 1][r - 1] == 0) return 1;
        }
        // L 第r列第i行 rth column, ith row of L
        for(int i = r + 1; i <= jieshu; i++){
            double sum = 0;
            for(int k = 1; k <= r - 1; k++){
                sum += L[i - 1][k - 1] * U[k - 1][r - 1];
            }
            L[i - 1][r - 1] = (A[i - 1][r - 1] - sum) / U[r - 1][r - 1];
        }
    }
    for(int r = 1; r <= jieshu - 1; r++){
    }
    // 解y
    double y[jieshu];
    y[1 - 1] = b[1 - 1];
    // y 第r行
    for(int r = 2; r <= jieshu; r++){
        double sum = 0;
        for(int j = 1; j <= r - 1; j++){
            sum += L[r - 1][j - 1] * y[j - 1];
        }
        y[r - 1] = b[r - 1] - sum;
    }
    // 解x
    x[jieshu - 1] = y[jieshu - 1] / U[jieshu - 1][jieshu - 1];
    // x 第r行
    for(int r = jieshu - 1; r >= 1; r--){
        double sum = 0;
        for(int j = r + 1; j <= jieshu; j++){
            sum += U[r - 1][j - 1] * x[j - 1];
        }
        x[r - 1] = (y[r - 1] - sum) / U[r - 1][r - 1];
    }
    return 0;
}

/*
 * 对于多项式函数类作为插值函数
 *
 *  v(x) = a0 + a1 * X + a2 * X^2 + ... + an * X^n
 *
 * 输入 函数表
 *
 *  X | x1 x2 ... xn
 *  Y | y1 y2 ... yn
 *
 * 输入函数类阶数
 *  
 *  qui
* */

int zuixiaoercheng(const int qui, // 阶数
                   const int sam, // 样本数
                   double * x, // 函数表
                   double * y, // 函数表
                   double * a){ // 输出系数
                               double A[qui + 1][qui + 1];
                               double sum[qui * 2 + 1];
                               sum[0] = sam;
                               // 计算各种和
                               for(int i = 1; i <= qui * 2; i++){
                                   sum[i] = 0;
                                   for(int j = 0; j < sam; j++){
                                       sum[i] += pow(x[j], i);
                                   }
                               }
                               // 给系数矩阵赋值
                               for(int i = 0; i <= qui; i++){
                                   for(int j = 0; j <= qui; j++){
                                       A[i][j] = sum[i + j];
                                   }
                               }
                               double b[qui + 1];
                               for(int i = 0; i <= qui; i++){
                                   double sum = 0;
                                   for(int j = 0; j < sam; j++){
                                       sum += pow(x[j], i) * y[j];
                                   }
                                   b[i] = sum;
                               }
                               if(lufenjiefa(qui + 1, (void *)A, a, b)){
                                   return 1;
                               }else{
                                   // success
                               }
                               return 0;
                  }

int main(void){
    double x[5] = {0.1, 0.2, 0.3, 0.4, 0.5};
    double y[5] = {30, 60, 95, 140, 215};
    // 根据图像拟分别采用三种多项式
    double a[4]; 
    if(zuixiaoercheng(1, 5, x, y, a)){
        puts("Error");
    }else{
        printf("S(x) = (%lf) + (%lf) * x\n", a[0], a[1]);
    }
    if(zuixiaoercheng(2, 5, x, y, a)){
        puts("Error");
    }else{
        printf("S(x) = (%lf) + (%lf) * x + (%lf) * x^2\n", a[0], a[1], a[2]);
    }
    if(zuixiaoercheng(3, 5, x, y, a)){
        puts("Error");
    }else{
        printf("S(x) = (%lf) + (%lf) * x + (%lf) * x^2 + (%lf) * x^3\n", a[0], a[1], a[2], a[3]);
    }
    return 0;
}
