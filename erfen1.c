/*************************************************************************
	> File Name: erfen1.c
	> Author: Zhu Brocadesoar
	> Mail: brocadesoar@gmail.com
	> Created Time: Tue 26 Sep 2017 07:18:39 AM CST
 ************************************************************************/

#include<stdio.h>
#include<math.h>

double a = 0, b = 0, c = 0, d = 0;// ax^3+bx^2+cx+d=0
double enda = 0, endb = 1;// (enda,endb)
int epsId = 2;// 0.5*10^(-epsId)

double func(double x){
    return a * pow(x, 3) + b * pow(x, 2) + c * x + d;
}

int main(void){

    puts("该程序用二分法计算区间(enda,endb)上三次方程ax^3+bx^2+cx+d=0的根，精度为0.5*10^(-epsId)");
    puts("确保输入的区间内至多一个根，否则结果无意义");

    puts("输入并回车：");
    printf("a: ");scanf("%lf", &a);
    printf("b: ");scanf("%lf", &b);
    printf("c: ");scanf("%lf", &c);
    printf("d: ");scanf("%lf", &d);
    printf("enda: ");scanf("%lf", &enda);
    printf("endb: ");scanf("%lf", &endb);
    printf("epsId: ");scanf("%d", &epsId);

    puts("==========输入参数如下==========");
    printf("(%.3lf)x^3 + (%.3lf)x^2 + (%.3lf)x + (%.3lf)\n", a, b, c, d);
    printf("(%.3lf,%.3lf),eps = 0.5*10^(-%d)\n", enda, endb, epsId);

    puts("============开始逼近============");

    double tmpa = enda, tmpb = endb;// 赋值初始区间，保留输入参数不变
    double eps = 0.5 * pow(10, -epsId);// 计算存储精度待用
    double check = func(tmpa) * func(tmpb);// 检查区间内确实有根或端点有根
    if(check > 0){// 输入区间无实数根或参数错误
        puts("若输入参数符合要求，则此输入区间无根");
        return 1;
    }else if(check == 0){// 区间端点含实数根
        if(func(tmpa) == 0){// 左端点为根
            printf("区间端点enda为一精确实数根: x = %.10lf\n", tmpa);
        }
        if(func(tmpb) == 0){// 右端点为根
            printf("区间端点endb为一根精确实数根: x = %.10lf\n", tmpb);
        }
        return 2;
    }
    double mid;// 中点
    while(tmpb - tmpa > eps){// 精度不满足要求继续二分
        mid = (tmpa + tmpb) / 2;
        printf("(%.10lf, %.10lf), mid = %.10lf\n", tmpa, tmpb, mid);
        if(func(mid) == 0){// 检查中点恰好为根
            printf("区间中点mid为一精确实数根: x = %.10lf\n", mid);
            return 3;
        }
        if(func(tmpa) * func(mid) < 0){// (tmpa, mid)有根，(mid, tmpb)无根
            tmpb = mid;
        }else{// (tmpa, mid)无根，(mid, tmpb)有根
            tmpa = mid;
        }
    }
    printf("区间内有一近似根: x = %.10f\n", mid);

    return 0;
}

