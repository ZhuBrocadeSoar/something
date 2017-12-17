/*************************************************************************
	> File Name: romberg.c
	> Author: Zhu Brocadesoar
	> Mail: brocadesoar@gmail.com
	> Created Time: Mon 18 Dec 2017 01:24:14 AM CST
 ************************************************************************/

#include<stdio.h>
#include<math.h>

#define DEBUGFLAG 1

/*
 * 此处定义积分函数
* */
double f(double x){
    return exp(x);
}

/*
 * 龙贝格函数
* */
double romberg(double (*f)(double x), // 积分函数的指针
               double a, // 积分下限
               double b, // 积分上限
               int n){ // 梯形复化分区数量
                      if(DEBUGFLAG){
                          puts("DEBUG模式");
                      }
                      double h = b - a;
                      // 梯形公式
                      double Tz = h / 2 * ((*f)(a) + (*f)(b));
                      if(DEBUGFLAG){
                          printf("T = %lf\n", Tz);
                      }
                      //用于记录复化结果的数组 T[0] = Tn; T[1] = T2n; T[2] = T4n; T[3] = T8n;
                      double T[4];
                      if(DEBUGFLAG){
                          puts("T[0] = Tn; T[1] = T2n; T[2] = T4n; T[3] = T8n;");
                      }
                      // double Tn, T2n, T4n, T8n;
                      // 梯形复化
                      /*
                      Tn = Tz;
                      for(int k = 1; k <= n - 1; k++){
                          Tn += h * (*f)(a + k * h / n);
                      }
                      Tn /= 2;
                      T2n = Tn;
                      for(int k = 1; k <= n; k++){
                          T2n += h / 2 * (*f)(a + (2 * k - 1) * h / 2 / n);
                      }
                      T2n /= 2;
                      T4n = T2n;
                      for(int k = 1; k <= 2 * n; k++){
                          T4n += h / 4 * (*f)(a + (2 * k - 1) * h / 4 / n); 
                      }
                      T4n /= 2;
                      T8n = T4n;
                      for(int k = 1; k <= 4 * n; k++){
                          T8n += h / 8 * (*f)(a + (2 * k - 1) * h / 8 / n);
                      }
                      T8n /= 2;
                      */
                      T[0] = Tz;
                      for(int k = 1; k <= n - 1; k++){
                          T[0] += h * (*f)(a + k * h / n);
                      }
                      T[0] /= 2;
                      if(DEBUGFLAG){
                          printf("T[%d] = %lf\n", 0, T[0]);
                      }
                      for(int i = 1; i <= 3; i++){
                          T[i] = T[i - 1];
                          for(int k = 1; k <= (pow(2, i - 1) * n); k++){
                              T[i] += h / pow(2, i) * (*f)(a + (2 * k - 1) * h / pow(2, i) / n);
                          }
                          T[i] /= 2;
                          if(DEBUGFLAG){
                              printf("T[%d] = %lf\n", i, T[i]);
                          }
                      }
                      // 辛普森复化结果 S[0] = Sn; S[1] = S2n; S[2] = S3n;
                      double S[3];
                      if(DEBUGFLAG){
                          puts("S[0] = Sn; S[1] = S2n; S[2] = S3n;");
                      }
                      for(int i = 0; i < 3; i++){
                          // 辛普森复化
                          S[i] = 4.0 / 3 * T[i + 1] - 1.0 / 3 * T[i];
                          if(DEBUGFLAG){
                              printf("S[%d] = %lf\n", i, S[i]);
                          }
                      }
                      // 柯斯特复化结果 C[0] = Cn; C[1] = C2n;
                      double C[2];
                      if(DEBUGFLAG){
                          puts("C[0] = Cn; C[1] = C2n;");
                      }
                      for(int i = 0; i < 2; i++){
                          // 柯斯特复化
                          C[i] = 16.0 / 15 * S[i + 1] - 1.0 / 15 * S[i];
                          if(DEBUGFLAG){
                              printf("C[%d] = %lf\n", i, C[i]);
                          }
                      }
                      // 龙贝格复化结果 Rn
                      double Rn;
                      Rn = 64.0 / 63 * C[1] - 1.0 / 63 * C[0];
                      if(DEBUGFLAG){
                          printf("Rn = %lf\n", Rn);
                      }
                      return Rn;
}

int main(void){
    double a = 0; // 积分下限
    double b = 1; // 积分上限
    int n = 2; // 初始梯形复化分区数
    printf("对于函数f(x) = exp(x)的(%lf, %lf)定积分\n", a, b);
    printf("初始梯形复化分区数n = %d\n龙贝格积分结果 Rn = %lf\n", n, romberg(f, a, b, n));
    return 0;
}
