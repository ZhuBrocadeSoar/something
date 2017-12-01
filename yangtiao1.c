/*************************************************************************
	> File Name: nihe.c
	> Author: Zhu Brocadesoar
	> Mail: brocadesoar@gmail.com
	> Created Time: Fri 01 Dec 2017 09:48:54 AM CST
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
 * 追赶法
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

// 返回非0值则方程奇异
int zhuiganfa(int jieshu, // 阶数 
               double * va, // 一维数组名
               double * vb, // 一维数组名
               double * vc, // 一维数组名
               double * vx, // 一维数组名
               double * vf){ // 一维数组名
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

/*
 * 三次样条
 * 输入函数表
 *
 *  x | x0 x1 x2 ... xn
 *  y | y0 y1 y2 ... yn
 *
 * 输入一类边界条件
 *
 *  y0d, ynd
 *
 * 输出构造系数向量
 *
 *  M0 M1 M2 ... Mn
 *
 * 拟合函数
 *
 *  S_i(x) = M_{i-1} * \frac{(x_i - x)^3}{6h_i} 
 *         + M_i * \frac{(x - x_i)^3}{6h_i}
 *         + (y_{i-1} - M_{i-1} / 6 * h_i^2) * \frac{x_i - x}{h_i}
 *         + (y_i - M_i / 6 * h_i^2) * \frac{x - x_{i-1}}{h_i}
 *         (i = 1, 2, ... n; x_{i-1} < x < x_i)
 *
 * 系数矩阵
 *
 *  S(i, X) = xa * X^3 + xb * X^2 + xc * X + xd
 *
* */

int yangtiao1(const int qui, // 区间数量n, 样本个数必须为 n+1 个(x0到xn)
              double * x, // 函数表x数组名
              double * y, // 函数表y数组名
              const double y0d, // 一类边界条件
              const double ynd, // 一类边界条件
              double * M, // 输出构造系数数组名
              double * xa, // 三次方系数
              double * xb, // 二次方系数
              double * xc, // 一次方系数
              double * xd){ // 零次方系数
                          double h[qui + 1]; // 区间长度,预留h[0]无用
                          h[0] = 0;
                          for(int i = 1; i <= qui; i ++){
                              h[i] = x[i] - x[i - 1];
                          }
                          double a[qui + 1]; // 三对角的下对角,预留a[0]无用
                          a[0] = 0;
                          for(int i = 1; i <= qui - 1; i++){
                              a[i] = h[i] / (h[i] + h[i + 1]);
                          }
                          a[qui] = 1;
                          double b[qui + 1]; // 三对角的中对角，
                          for(int i = 0; i <= qui; i++){
                              b[i] = 2;
                          }
                          double c[qui + 1]; // 三对角的上对角，预留c[qui]无用
                          c[0] = 1;
                          for(int i = 1; i <= qui - 1; i++){
                              c[i] = 1 - a[i];
                          }
                          c[qui] = 0;
                          double g[qui+1]; // 三对角的尾向量
                          g[0] = 6 / h[1] * ((y[1] - y[0]) / h[1] - y0d);
                          for(int i = 1; i <= qui - 1; i++){
                              g[i] = 6 / (h[i] + h[i + 1]) * ((y[i + 1] - y[i]) / h[i + 1] - (y[i] - y[i - 1]) / h[i]);
                          }
                          g[qui] = 6 / h[qui] * (ynd - (y[qui] - y[qui - 1]) / h[qui]);
                          if(zhuiganfa(qui + 1, a, b, c, M, g)){
                              // 调用追赶法，遇到奇异方程
                              return 1;
                          }else{
                              // 调用追赶法，计算插值方程系数
                              for(int i = 1; i <= qui; i++){
                                  xa[i] = (M[i] - M[i - 1]) / (6 * h[i]);
                                  xb[i] = (M[i - 1] * x[i] - M[i] * x[i - 1]) / (2 * h[i]);
                                  xc[i] = (h[i] * M[i - 1] - h[i] * M[i]) / 6 + (y[i] - y[i - 1]) / h[i] + (M[i] * x[i - 1] * x[i - 1] - M[i - 1] * x[i] * x[i]) / (2 * h[i]);
                                  xd[i] = (M[i - 1] * x[i] * x[i] * x[i] - M[i] * x[i - 1] * x[i - 1] * x[i - 1]) / (6 * h[i]) + (h[i] * M[i] * x[i - 1] - h[i] * M[i - 1] * x[i]) / 6 + (x[i] * y[i - 1] - x[i - 1] * y[i]) / h[i];
                              }
                          }
    return 0;
}

/*
* 根据输入的插值函数系数和待插值计算的值
* 将返回结果到指针指向*/
int eveal(int qui, // 分段区间数量
          double * xa, // 三次方系数 数组名
          double * xb, // 二次方系数 数组名
          double * xc, // 一次方系数 数组名
          double * xd, // 零次方系数 数组名
          double * x, // 区间端点数组 数组名
          double val, // 待求值
          double * ret){ // 结果值
              int flag = 0;
              for(int i = 1; i <= qui; i++){
                  if(val < x[i - 1]){
                      // 小于最小区间左端点，结果无意义
                      return 1;
                  }else if(val <= x[i]){
                      flag = i;
                      break;
                  }
              }
              if(flag == 0){
                  // 大于最大区间右端点，结果无意义
                  return 2;
              }else{
                  // 计算
                  *ret = xa[flag] * val * val * val + xb[flag] * val * val + xc[flag] * val + xd[flag];
              }
              return 0;
          }

int main(void){
    /*
     * 由于课本例题4.6.5关于spline和interp1函数的介绍未给出边界条件的处理方法,
     * 有对于另一例题的探究如下
    * 对于课本例题4.4.1
    * */
    double x[5] = {0, 0.15, 0.30, 0.45, 0.60}; // 函数表
    double y[5] = {1, 0.978, 0.91743, 0.83160, 0.73529}; // 函数表
    double M[5]; // 存放 构造系数 向量
    double xa[5], xb[5], xc[5], xd[5]; // 存放 插值函数 系数矩阵
    double y0d = 0; // 第一类边界条件
    double ynd = -0.64879; // 第一类边界条件
    // 打印函数表
    for(int i = 0; i <= 4; i++){
        printf("x[%d] = %lf\t", i, x[i]);
    }
    printf("\n");
    for(int i = 0; i <= 4; i++){
        printf("y[%d] = %lf\t", i, y[i]);
    }
    printf("\n");
    // 打印插值结果
    if(yangtiao1(4, x, y, y0d, ynd, M, xa, xb, xc, xd)){
        puts("Error");
    }else{
        for(int i = 0; i <= 4; i++){
            printf("M[%d] = %lf\t", i, M[i]);
        }
        printf("\n");
        for(int i = 1; i <= 4; i++){
            printf("S(%d, X) = (%lf) X^3 + (%lf) X^2 + (%lf) X + (%lf)\n", i, xa[i], xb[i], xc[i], xd[i]);
        }
        printf("\n");
        // 计算待求插值
        double x0 = 0.2;
        double y0;
        if(eveal(4, xa, xb, xc, xd, x, x0, &y0)){
            printf("x = %lf is not in range\n", x0);
        }else{
            printf("y = f(%lf) = %lf\n", x0, y0);
            printf("与课本给出的答案对比，M的值和各系数均有差别，最终计算结果单0.2这一点来看精度还是可以的，数据出入的原因尚未明确，有可能是算法的差异或者基于不同语言的过程差异\n");
        }
    }
    return 0;
}

