/*************************************************************************
	> File Name: 064朱锦腾_第一次作业.c
	> Author: Zhu Brocadesoar
	> Mail: brocadesoar@gmail.com
	> Created Time: Sun 08 Oct 2017 12:43:46 AM CST
 ************************************************************************/

#include<stdio.h>
#include<math.h>
#include<gsl/gsl_poly.h>

// 三次方程参数设置
// 对于三次方程 x^3 + ax^2 + bx + c = 0
// 有三个参数 a, b, c
double a = 0, b = 0, c = 0;

// 有最多三个实数解
double x[3];

// 二分法输入参数设置
double enda = 0, endb = 1;// (enda, endb)
int epsId = 2;// eps = 0.5*10^(-epsId)

// 原方程函数
double func(double x){
    return pow(x, 3) + a * pow(x, 2) + b * x + c;
}

// 一种一般迭代函数
double funcDiedai(double x){
    return pow(- a * pow(x, 2) - b * x - c, 1.0 / 3.0);
}

int main(void){
    // 第一阶段，输入待求解方程参数
    puts("对于三次方程 x^3 + ax^2 + bx + c = 0");
    puts("输入参数并回车:");
    printf("a = ");scanf("%lf", &a);
    printf("b = ");scanf("%lf", &b);
    printf("c = ");scanf("%lf", &c);
    puts("您输入的方程是");
    printf("x^3 + (%.5lf)x^2 + (%.5lf)x + (%.5lf) = 0\n", a, b, c);

    // 第二阶段，调用GSL库计算可靠解
    puts("调用GSL库计算可靠的实数解");
    int rootNum = gsl_poly_solve_cubic(a, b, c, &x[0], &x[1], &x[2]);
    for(int i = 0; i < rootNum; i++){
        printf("x[%d] = %.10lf\n", i, x[i]);
    }

    // 第三阶段，二分法
    puts("试用用二分法求解区间(enda, endb)上该方程的根");
    puts("求根精度为0.5*10^(-epsId)");
    puts("根据已知可靠解正确输入二分法参数:");
    printf("enda = ");scanf("%lf", &enda);
    printf("endb = ");scanf("%lf", &endb);
    printf("epsId = ");scanf("%d", &epsId);
    puts("您输入的区间是");
    printf("(%.5lf, %.5lf)\n", enda, endb);
    puts("您输入的精度是");
    printf("0.5 * 10^(-%d)\n", epsId);

    puts("开始逼近");

    double tmpa = enda, tmpb = endb;// 赋值初始区间，保留输入参数不变
    double eps = 0.5 * pow(10, - epsId);// 计算存储精度待用
    double check = func(tmpa) * func(tmpb);// 检查区间内确实有实数根或者端点有实数根
    if(check > 0){// 输入区间无实数根或者参数错误
        puts("若输入参数符合要求，则此输入区间无根");
        return 1;
    }else if(check == 0){// 区间端点含实数根
        if(func(tmpa) == 0){// 左端点为根
            printf("区间端点enda为一实数根: x = %.10lf\n", tmpa);
        }
        if(func(tmpb) == 0){// 右端点为根
            printf("区间端点endb为一实数根: x = %.10lf\n", tmpb);
        }
        return 2;
    }
    double mid;// 中点
    while(tmpb - tmpa > eps){// 精度不满足要求继续二分
        mid = (tmpa + tmpb) / 2;
        printf("(%.10lf, %.10lf), mid = %.10lf\n", tmpa, tmpb, mid);
        if(func(mid) == 0){// 检查中点恰好为根
            printf("区间中点mid为一实数根: x = %.10lf\n", mid);
            return 3;
        }
        if(func(tmpa) * func(mid) < 0){// (tmpa, mid)有根, (mid, tmpb)无根
            tmpb = mid;
        }else{// (tmpa, mid)无根, (mid, tmpb)有根
            tmpa = mid;
        }
    }
    printf("区间内有一近似根: x = %.10lf\n", mid);

    // 第四阶段，一般迭代法
    puts("试用一般迭代法求该方程的实数根");
    puts("迭代函数为");
    printf("x[*] = (- (%.5lf)x^2 - (%.5lf)x - (%.5lf))^(1/3)\n", a, b, c);
    puts("根据已知可靠解输入迭代初始值(精度同二分法输入不再输入)：");
    double x0 = 0, xtmp;// 迭代初始值, 迭代过程暂存
    int num = 0;// 迭代次数
    printf("x[0] = ");scanf("%lf", &x0);
    puts("开始迭代");
    while(1){
        xtmp = funcDiedai(x0);
        printf("x[%d] = %.10lf\n", ++num, xtmp);
        /*  计算并检查精度？
        if(abs(x0 - xtmp) < eps){
            puts("精度满足要求结束迭代");
            break;
        }
        */
        if(num >= 20){
            //puts("迭代次数超过1000次,可能此迭代函数对于输入的初始值不收敛");
            puts("迭代20次完成");
            break;
        }
        x0 = xtmp;
    }

    return 0;
}

