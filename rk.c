/*************************************************************************
	> File Name: rk.c
	> Author: Zhu Brocadesoar
	> Mail: brocadesoar@gmail.com
	> Created Time: Sun 24 Dec 2017 07:35:55 AM CST
 ************************************************************************/

#include<stdio.h>
#include<math.h>

#define DEBUGFLAG 1

/*
 * 此处定义常微分方程
 * f'(x) = g(x, f(x))
 * 范围
 * 0 <= x <= 1.2
 * 初始条件
 * f(0) = y0;
 * */
double fd(double x, double y){
    return -2 * x * y;
}

int rk(double (*fd)(double xx, double yy), // 常微分方程函数的指针
       double a, // 区间左端点
       double b, // 区间右端点
       const int n, // 区间分段数量
       double y0, // 初始点
       double * x, // x向量数组名
       double * y){ // y向量数组名
                  if(DEBUGFLAG){
                      puts("DEBUG模式");
                  }
                  double h = (b - a) / n; // 分段区间长度
                  double k1, k2, k3, k4;
                  y[0] = y0;
                  for(int i = 0; i <= n; i++){
                      x[i] = a + i * h;
                  }
                  for(int i = 1; i <= n; i++){
                      k1 = (*fd)(x[i - 1], y[i - 1]);
                      k2 = (*fd)(x[i - 1] + 1.0 / 2 * h, y[i - 1] + 1.0 / 2 * h * k1);
                      k3 = (*fd)(x[i - 1] + 1.0 / 2 * h, y[i - 1] + 1.0 / 2 * h * k2);
                      k4 = (*fd)(x[i - 1] + h, y[i - 1] + h * k3);
                      y[i] = y[i - 1] + h / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4);
                      if(DEBUGFLAG){
                          printf("k1 = %lf\tk2 = %lf\tk3 = %lf\tk4 = %lf\n", k1, k2, k3, k4);
                          printf("x[%d] = %lf\t\ty[%d] = %lf\n", i, x[i], i, y[i]);
                      }
                  }
    return 0;
}

int main(void){
    const int qujianshu = 6;
    double a = 0, b = 1.2;
    double y0 = 1;
    double x[qujianshu + 1];
    double y[qujianshu + 1];
    puts("对常微分方程f'(x) = g(x, f(x)); 0 <= x <= 1.2");
    puts("f(0) = 1;");
    puts("用四阶R-K法计算分段区间端点对应的点");
    puts("这些点的拟合近似于该常微分方程的解");
    rk(fd, a, b, qujianshu, y0, x, y);
    puts("打印结果如下");
    for(int i = 0; i <= qujianshu; i++){
        printf("x[%d] = %lf\t\ty[%d] = %lf\n", i, x[i], i, y[i]);
    }
    return 0;
}
